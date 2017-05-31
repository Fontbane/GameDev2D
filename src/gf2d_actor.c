#include "gf2d_actor.h"
#include "simple_logger.h"

#include <stdio.h>

typedef struct
{
    Uint32 maxActionLists;
    ActionList * actionLists;
}ActionManager;

static ActionManager action_list_manager;


void gf2d_action_list_clear_all();

void gf2d_action_list_close()
{
    gf2d_action_list_clear_all();
    if (action_list_manager.actionLists != NULL)
    {
        free(action_list_manager.actionLists);
    }
    action_list_manager.actionLists = NULL;
    action_list_manager.maxActionLists = 0;
    slog("actionList system closed");
}

void gf2d_action_list_init(Uint32 max)
{
    if (!max)
    {
        slog("cannot intialize a actionList manager for Zero actionLists!");
        return;
    }
    action_list_manager.maxActionLists = max;
    action_list_manager.actionLists = (ActionList *)malloc(sizeof(ActionList)*max);
    memset (action_list_manager.actionLists,0,sizeof(ActionList)*max);
    slog("Action system initialized");
    atexit(gf2d_action_list_close);
}

void gf2d_action_list_delete(ActionList *actionList)
{
    if (!actionList)return;
    if (actionList->actions != NULL)
    {
        free(actionList->actions);
    }    
    memset(actionList,0,sizeof(ActionList));//clean up all other data
}

void gf2d_action_list_free(ActionList *actionList)
{
    if (!actionList) return;
    actionList->ref_count--;
}

void gf2d_action_list_clear_all()
{
    int i;
    for (i = 0;i < action_list_manager.maxActionLists;i++)
    {
        gf2d_action_list_delete(&action_list_manager.actionLists[i]);// clean up the data
    }
}

ActionList *gf2d_action_list_new()
{
    int i;
    /*search for an unused sprite address*/
    for (i = 0;i < action_list_manager.maxActionLists;i++)
    {
        if ((action_list_manager.actionLists[i].ref_count == 0)&&(action_list_manager.actionLists[i].actions == NULL))
        {
            action_list_manager.actionLists[i].ref_count = 1;//set ref count
            return &action_list_manager.actionLists[i];//return address of this array element        }
        }
    }
    /*find an unused sprite address and clean up the old data*/
    for (i = 0;i < action_list_manager.maxActionLists;i++)
    {
        if (action_list_manager.actionLists[i].ref_count == 0)
        {
            gf2d_action_list_delete(&action_list_manager.actionLists[i]);// clean up the old data
            action_list_manager.actionLists[i].ref_count = 1;//set ref count
            return &action_list_manager.actionLists[i];//return address of this array element
        }
    }
    slog("error: out of sprite addresses");
    return NULL;
}

ActionList *gf2d_action_list_get_by_filename(char * filename)
{
    int i;
    for (i = 0;i < action_list_manager.maxActionLists;i++)
    {
        if (gf2d_line_cmp(action_list_manager.actionLists[i].filename,filename)==0)
        {
            return &action_list_manager.actionLists[i];
        }
    }
    return NULL;// not found
}


int gf2d_action_file_get_count(FILE *file)
{
    char buf[512];
    int count = 0;
    if (!file)return 0;
    rewind(file);
    while(fscanf(file, "%s", buf) != EOF)
    {
        if(strcmp(buf,"action:") == 0)
        {
            count++;
        }
        fgets(buf, sizeof(buf), file);
    }
    return count;
}

void gf2d_action_file_load_actions(FILE *file,Action *actions)
{
    char buf[512];
    if (!file)return;
    rewind(file);
    actions--;
    while(fscanf(file, "%s", buf) != EOF)
    {
        if(strcmp(buf,"action:") == 0)
        {
            actions++;
            fscanf(file,"%s",(char*)&actions->name);
            continue;
        }
        if(strcmp(buf,"startFrame:") == 0)
        {
            fscanf(file,"%i",&actions->startFrame);
            continue;
        }
        if(strcmp(buf,"endFrame:") == 0)
        {
            fscanf(file,"%i",&actions->endFrame);
            continue;
        }
        if(strcmp(buf,"frameRate:") == 0)
        {
            fscanf(file,"%f",&actions->frameRate);
            continue;
        }
        fgets(buf, sizeof(buf), file);
    }
}

ActionList *gf2d_action_list_load(
    char *filename
)
{
    FILE *file;
    ActionList *actionList;
    int count;
    actionList = gf2d_action_list_get_by_filename(filename);
    if (actionList != NULL)
    {
        // found a copy already in memory
        actionList->ref_count++;
        return actionList;
    }
    
    actionList = gf2d_action_list_new();
    if (!actionList)
    {
        return NULL;
    }
    file = fopen(filename,"r");
    if (!file)
    {
        gf2d_action_list_delete(actionList);
        slog("failed to open action file: %s",filename);
        return NULL;
    }
    gf2d_line_cpy(actionList->filename,filename);
    count = gf2d_action_file_get_count(file);
    if (!count)
    {
        gf2d_action_list_delete(actionList);
        fclose(file);
        slog("No actions found in file: %s",filename);
        return NULL;
    }
    actionList->actions = (Action*)malloc(sizeof(Action)*count);
    memset(actionList->actions,0,sizeof(Action)*count);
    actionList->numActions = count;
    gf2d_action_file_load_actions(file,actionList->actions);
    
    fclose(file);
    return actionList;
}

Action *gf2d_action_list_get_action(ActionList *al, char *name)
{
    int i;
    if (!al)
    {
        slog("no action list provided");
        return NULL;
    }
    if (!name)
    {
        slog("no filename provided");
        return NULL;
    }
    for (i = 0; i < al->numActions;i++)
    {
        if (gf2d_line_cmp(al->actions[i].name,name) == 0)
        {
            return &al->actions[i];
        }
    }
    return NULL;// not found
}

ActionReturnType gf2d_action_list_get_next_frame(
    ActionList *al,
    float * frame,
    char *name)
{
    Action *action;
    if (!frame)
    {
        slog("missing frame data");
        return ART_ERROR;
    }
    action = gf2d_action_list_get_action(al, name);
    if (!action)
    {
        slog("no action found by name %s",name);
        return ART_ERROR;
    }
    if ((*frame < action->startFrame)||(*frame > action->endFrame))
    {
        *frame = action->startFrame;
        return ART_START;
    }
    *frame += action->frameRate;
    if (*frame > action->endFrame)
    {
        switch (action->type)
        {
            case AT_NONE:
                break;
            case AT_LOOP:
                *frame = action->startFrame;
                break;
            case AT_PASS:
                *frame = action->endFrame;
                return ART_END;
        }
    }
    return ART_NORMAL;
}

/*eol@eof*/
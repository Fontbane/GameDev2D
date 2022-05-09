#include <stdlib.h>
#include <stdio.h>
#include "simple_logger.h"
#include "gfc_text.h"
#include "gf2d_font.h"
#include "gf2d_element_label.h"
#include "k_hud.h"

void gf2d_element_label_draw(Element *element,Vector2D offset)
{
    MessageBox *label;
    Vector2D position;
    if (!element)return;
    label = (MessageBox*)element->data;
    if (!label)return;
    vector2d_add(position, offset, element->bounds);
    if (gfc_line_cmp(element->name, "msgbox_lines") == 0) {
        gf2d_font_draw_line_tag(label->lines[label->state], FT_Normal, element->color, position);
    }
    else if (gfc_line_cmp(element->name, "msgbox_title") == 0) {
        gf2d_font_draw_line_tag(label->title, FT_Normal, element->color, position);
    }
    // adjust position to top left
}

List *gf2d_element_label_update(Element *element,Vector2D offset)
{
    if (gfc_line_cmp(element->name, "msgbox_lines") == 0) {
        const Uint8* keys;
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        SDL_Event keyEvent;
        SDL_PollEvent(&keyEvent);

        MessageBox* msg = (MessageBox*)element->data;

        if (keys[SDL_SCANCODE_SPACE]) {
            if (++(msg->state) == msg->numlines) {
                gf2d_element_free(element);
                return NULL;
            }
            
        }
    }
    return NULL;
}

void gf2d_element_label_free(Element *element)
{
    MessageBox *label;
    if (!element)return;
    label = (MessageBox*)element->data;
    if (label != NULL)
    {
        free(label);
    }
}

LabelElement *gf2d_element_label_new()
{
    LabelElement *label;
    label = (LabelElement *)malloc(sizeof(LabelElement));
    if (!label)
    {
        slog("failed to allocate memory for label");
        return NULL;
    }
    memset(label,0,sizeof(LabelElement));
    return label;
}


LabelElement *gf2d_element_label_new_full(char *text,Color color,int style,int justify,int align,int wraps)
{
    LabelElement *label;
    label = gf2d_element_label_new();
    if (!label)
    {
        return NULL;
    }
    gfc_block_cpy(label->text,text);
    label->bgcolor = color;
    label->style = style;
    label->justify = justify;
    label->alignment = align;
    label->wraps = wraps;
    return label;
}

void gf2d_element_make_label(Element *e,LabelElement *label)
{
    if (!e)return;
    e->data = label;
    e->type = ET_Label;
    e->draw = gf2d_element_label_draw;
    e->update = gf2d_element_label_update;
    e->free_data = gf2d_element_label_free;
}

const char *gf2d_element_label_get_text(Element *e)
{
    if (!e)return NULL;
    if (e->type != ET_Label)return NULL;
    LabelElement *label;
    label = (LabelElement *)e->data;
    if (!label)return NULL;
    return label->text;
}

void gf2d_element_label_set_text(Element *e,char *text)
{
    if (!e)return;
    if (e->type != ET_Label)return;
    LabelElement *label;
    label = (LabelElement *)e->data;
    if (!label)return;
    gfc_block_cpy(label->text,text);
}

void gf2d_element_load_label_from_config(Element *e,SJson *json)
{
    SJson *value;
    Vector4D vector;
    Color color;
    const char *buffer;
    int style = FT_Normal;
    int justify = LJ_Left;  
    int align = LA_Top;
    int wraps = 0;
    if ((!e) || (!json))
    {
        slog("call missing parameters");
        return;
    }
    value = sj_object_get_value(json,"style");
    buffer = sj_get_string_value(value);
    if (buffer)
    {
        if (strcmp(buffer,"normal") == 0)
        {
            style = FT_Normal;
        }
        else if (strcmp(buffer,"small") == 0)
        {
            style = FT_Small;
        }
        else if (strcmp(buffer,"H1") == 0)
        {
            style = FT_H1;
        }
        else if (strcmp(buffer,"H2") == 0)
        {
            style = FT_H2;
        }
        else if (strcmp(buffer,"H3") == 0)
        {
            style = FT_H3;
        }
        else if (strcmp(buffer,"H4") == 0)
        {
            style = FT_H4;
        }
        else if (strcmp(buffer,"H5") == 0)
        {
            style = FT_H5;
        }
        else if (strcmp(buffer,"H6") == 0)
        {
        style = FT_H6;
        }
    }

    value = sj_object_get_value(json,"wraps");
    if (value)
    {
        sj_get_bool_value(value,(short int *)&wraps);
    }
    value = sj_object_get_value(json,"justify");
    buffer = sj_get_string_value(value);
    if (buffer)
    {
        if (strcmp(buffer,"left") == 0)
        {
            justify = LJ_Left;
        }
        else if (strcmp(buffer,"center") == 0)
        {
            justify = LJ_Center;
        }
        else if (strcmp(buffer,"right") == 0)
        {
            justify = LJ_Right;
        }
    }

    value = sj_object_get_value(json,"align");
    buffer = sj_get_string_value(value);
    if (buffer)
    {
        if (strcmp(buffer,"top") == 0)
        {
            align = LA_Top;
        }
        else if (strcmp(buffer,"middle") == 0)
        {
            align = LA_Middle;
        }
        else if (strcmp(buffer,"bottom") == 0)
        {
            align = LA_Bottom;
        }
    }
    value = sj_object_get_value(json,"color");
    vector4d_set(vector,255,255,255,255);
    sj_value_as_vector4d(value,&vector);
    color = gfc_color_from_vector4(vector);

    value = sj_object_get_value(json,"text");
    buffer = sj_get_string_value(value);
    gf2d_element_make_label(e,gf2d_element_label_new_full((char *)buffer,color,style,justify,align,wraps));
}

/*eol@eof*/

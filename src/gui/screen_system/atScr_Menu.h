/**
@file
Screen Template.
*/
/*
  Screen

  Library:: 

  This version is

  Copyright:: 2021 nguyentrinhtuan1996@gmail.com
*/
#ifndef _atScr_Menu_
#define _atScr_Menu_
/* _____PROJECT INCLUDES____________________________________________________ */

#include "Arduino.h"
#include "lvgl.h"
#include "../Menu_Screen.h"
#include "../Sources.h"
/* _____GLOBAL VARIABLES_____________________________________________________ */

/* _____DEFINETIONS__________________________________________________________ */

/*____CLASS DEFINITION_____________________________________________________ */
/**
 * This Screen class is base class for implement another class by inheritance 
 */
class Scr_Menu: public Menu_Screen, public Sources
{
public:
    Scr_Menu();
    ~Scr_Menu();
	static void  Start();	
	static void  Execute();
protected:    
	void Update();
private:
    static void btn_Back_event_handler(lv_event_t *e);
    static void btn_OK_event_handler(lv_event_t *e);
    
}atScr_Menu;

Scr_Menu::Scr_Menu()
{
    _Start_User     = *Start;
    _Execute_User   = *Execute;
    ID_Screen = 2;
    Name_Screen = (char*)"Menu Screen";
}
Scr_Menu::~Scr_Menu()
{
    
}
/**
 * This start function will init some critical function 
 */
void  Scr_Menu::Start()
{
	atScr_Menu.screen_status = ACTIVE;
    //Write codes screen
	atScr_Menu.Object = lv_obj_create(NULL);
	//Create 4 button
	atScr_Menu.create_buttons(atScr_Menu.Object);
	//Write codes screen_1_label_1
	atScr_Menu.label_screen = lv_label_create(atScr_Menu.Object);
	atScr_Menu.setup_label(atScr_Menu.label_screen,"Menu.title",31,30,100,32);
	//create notified bar
	atScr_Menu.init_notified_bar(atScr_Menu.Object);
	if(atScr_Menu.modbus_active_old == ON)
	{
		atScr_Menu.setup_label(atScr_Menu.Screen_label_modbus,LV_SYMBOL_REFRESH,45,0,15,15);
	}
	else atScr_Menu.setup_label(atScr_Menu.Screen_label_modbus,"  ",45,0,15,15);
	if(atScr_Menu.warning_active_old == ON)
	{
		atScr_Menu.setup_label(atScr_Menu.Screen_label_warning,LV_SYMBOL_WARNING,60,0,15,15);
	}
	else atScr_Menu.setup_label(atScr_Menu.Screen_label_warning,"  ",60,0,15,15);
	if(atScr_Menu.SD_active_old == ON)
	{
		atScr_Menu.setup_label(atScr_Menu.Screen_label_SD,LV_SYMBOL_SD_CARD,30,0,15,15);
	}
	else atScr_Menu.setup_label(atScr_Menu.Screen_label_SD,"  ",30,0,15,15);
	if(atScr_Menu.bluetooth_active_old == ON)
	{
		atScr_Menu.setup_label(atScr_Menu.Screen_label_bluetooth,LV_SYMBOL_BLUETOOTH,15,0,15,15);
	}
	else atScr_Menu.setup_label(atScr_Menu.Screen_label_bluetooth,"  ",15,0,15,15);
	if(atScr_Menu.wifi_active_old == ON)
	{
		atScr_Menu.setup_label(atScr_Menu.Screen_label_wifi,LV_SYMBOL_WIFI,0,0,15,15);
	}
	else atScr_Menu.setup_label(atScr_Menu.Screen_label_wifi,"  ",0,0,15,15);
	// init every events
    lv_obj_add_event_cb(atScr_Menu.btn_BACK, atScr_Menu.btn_Back_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(atScr_Menu.btn_OK, atScr_Menu.btn_OK_event_handler, LV_EVENT_ALL, NULL);
} 
/**
 * Execute fuction of SNM app
 */
void  Scr_Menu::Execute()
{
	atScr_Menu.Update();
}

void Scr_Menu::btn_Back_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		atScr_Menu.screen_status = DEACTIVE;
		if (!lv_obj_is_valid(*atScr_Menu.Backward_Screen))
        {
			(*atScr_Menu.setup_Backward_Screen)();
        }
		lv_disp_t * d = lv_obj_get_disp(lv_scr_act());
		if (d->prev_scr == NULL && d->scr_to_load == NULL)
        {
			lv_scr_load_anim(*atScr_Menu.Backward_Screen, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 100, true);
        }
	}
		break;
	default:
		break;
	}
}
void Scr_Menu:: btn_OK_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		atScr_Menu.screen_status = DEACTIVE;
		if (!lv_obj_is_valid(*atScr_Menu.Forward_Screen))
        {
			(*atScr_Menu.setup_Forward_Screen)();
        }
		lv_disp_t * d = lv_obj_get_disp(lv_scr_act());
		if (d->prev_scr == NULL && d->scr_to_load == NULL)
        {
			lv_scr_load_anim(*atScr_Menu.Forward_Screen, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);
        }
	}
		break;
	default:
		break;
	}
}
void Scr_Menu::Update()
{

	if (atScr_Menu.Notified_Bar_1.minute < 60)
	{
		atScr_Menu.Notified_Bar_1.minute ++;
	}
	else 
	{
		atScr_Menu.Notified_Bar_1.minute = 0;
		if(atScr_Menu.Notified_Bar_1.hour < 22)
			atScr_Menu.Notified_Bar_1.hour ++;
		else atScr_Menu.Notified_Bar_1.hour = 0;
	}
	sprintf(atScr_Menu.Notified_Bar_1.time,"%d:%d",atScr_Menu.Notified_Bar_1.hour,atScr_Menu.Notified_Bar_1.minute);
	atScr_Menu.setup_label(atScr_Menu.Screen_label_time,atScr_Menu.Notified_Bar_1.time,120,0,40,15);

	//check active wifi
	if(atScr_Menu.wifi_active != atScr_Menu.wifi_active_old)
	{
		atScr_Menu.wifi_active_old = atScr_Menu.wifi_active;
		if(atScr_Menu.wifi_active_old == ON)
		{
			atScr_Menu.setup_label(atScr_Menu.Screen_label_wifi,LV_SYMBOL_WIFI,0,0,15,15);
		}
		else atScr_Menu.setup_label(atScr_Menu.Screen_label_wifi,"  ",0,0,15,15);
	}
	// check active
	if(atScr_Menu.bluetooth_active != atScr_Menu.bluetooth_active_old)
	{
		atScr_Menu.bluetooth_active_old = atScr_Menu.bluetooth_active;
		if(atScr_Menu.bluetooth_active_old == ON)
		{
			atScr_Menu.setup_label(atScr_Menu.Screen_label_bluetooth,LV_SYMBOL_BLUETOOTH,15,0,15,15);
		}
		else atScr_Menu.setup_label(atScr_Menu.Screen_label_bluetooth,"  ",15,0,15,15);
		
	}
	//check active
	if (atScr_Menu.SD_active != atScr_Menu.SD_active_old)
	{
		atScr_Menu.SD_active_old = atScr_Menu.SD_active;
		if(atScr_Menu.SD_active_old == ON)
		{
			atScr_Menu.setup_label(atScr_Menu.Screen_label_SD,LV_SYMBOL_SD_CARD,30,0,15,15);
		}
		else atScr_Menu.setup_label(atScr_Menu.Screen_label_SD,"  ",30,0,15,15);
	}
	//check active
	if (atScr_Menu.modbus_active != atScr_Menu.modbus_active_old)
	{
		atScr_Menu.modbus_active_old = atScr_Menu.modbus_active;
		if(atScr_Menu.modbus_active_old == ON)
		{
			atScr_Menu.setup_label(atScr_Menu.Screen_label_modbus,LV_SYMBOL_REFRESH,45,0,15,15);
		}
		else atScr_Menu.setup_label(atScr_Menu.Screen_label_modbus,"  ",45,0,15,15);
	}	
	//check active
	if (atScr_Menu.warning_active != atScr_Menu.warning_active_old)
	{
		atScr_Menu.warning_active_old = atScr_Menu.warning_active;
		if(atScr_Menu.warning_active_old == ON)
		{
			atScr_Menu.setup_label(atScr_Menu.Screen_label_warning,LV_SYMBOL_WARNING,60,0,15,15);
		}
		else atScr_Menu.setup_label(atScr_Menu.Screen_label_warning,"  ",60,0,15,15);
	}
}
#endif
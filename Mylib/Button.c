#include "Button.h"

__weak void btn_pressing_callback(Button_Typedef *ButtonX)
{
}
__weak void btn_release_callback(Button_Typedef *ButtonX)
{
}
__weak void btn_longpress_callback(Button_Typedef *ButtonX)
{
}
__weak void btn_shortpress_callback(Button_Typedef *ButtonX)
{
}
void button_handle(Button_Typedef *ButtonX)
{
//---------------------------L?c nhi?u--------------------------------
	uint8_t button_state = HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin);
	if (button_state != ButtonX->button_filter)
	{
		ButtonX->button_filter = button_state;
		ButtonX->button_debounce = 1;
		ButtonX->time_debounce = HAL_GetTick();
	}
//-----------------------Tín hi?u dã xác l?p--------------------------
	if (ButtonX->button_debounce && (HAL_GetTick()- ButtonX->time_debounce >= 15))
	{
		ButtonX->button_current = ButtonX->button_filter;
		ButtonX->button_debounce = 0;
	}
//------------------------X? lí nh?n nh?------------------------------
	if (ButtonX->button_current != ButtonX->button_last)
	{

		if (ButtonX->button_current == 0) //nh?n nút
		{
			ButtonX->is_press_time_out = 1;
			btn_pressing_callback(ButtonX);
			ButtonX->time_start_press = HAL_GetTick();
		}
		else //nh? nút
		{
			if ( HAL_GetTick() - ButtonX->time_start_press <= 1000)
			{
				btn_shortpress_callback(ButtonX);
			}
			btn_release_callback(ButtonX);
			ButtonX->is_press_time_out = 0;
		}
		ButtonX->button_last = ButtonX->button_current;

	}
//----------------------------X? lí nh?n gi?------------------------------
	if (ButtonX->is_press_time_out && (HAL_GetTick() - ButtonX->time_start_press >= 3000))
	{
		btn_longpress_callback(ButtonX);
		ButtonX->is_press_time_out = 0;
	}

}
void button_init(Button_Typedef *ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	ButtonX->GPIOx = GPIOx;
	ButtonX->GPIO_Pin= GPIO_Pin;
}



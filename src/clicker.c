#include "clicker.h"
#include <dlog.h>
#include <device/callback.h>

typedef struct appdata{
	Evas_Object* win;
	Evas_Object* layout;
	Evas_Object* conform;
	Evas_Object* grid;
	Evas_Object* label;
	Evas_Object* buttonReload;
	Evas_Object* buttonPlus;
	Evas_Object* buttonMinus;
	Evas_Object* box;
} appdata_s;

#define INT_DIGITS 19
#define TEXT_BUF_SIZE 256
#define ICON_DIR "/opt/usr/apps/org.example.clicker/res/Images/"

int counter = 0;//добавить сохранение и поменять
char label_text[TEXT_BUF_SIZE];

char *itoa(i)
     int i;
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 2];
  char *p = buf + INT_DIGITS + 1;	/* points to terminating '\0' */
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			/* i < 0 */
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
layout_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

static void
app_get_resource(const char *edj_file_in, char *edj_path_out, int edj_path_max)
{
	char *res_path = app_get_resource_path();
	if (res_path) {
		snprintf(edj_path_out, edj_path_max, "%s%s", res_path, edj_file_in);
		free(res_path);
	}
}

_create_grid(appdata_s *ad)
{
	ad->grid = elm_grid_add(ad->conform);
	elm_grid_size_set(ad->grid, 360, 360);
	evas_object_show(ad->grid);//отображение контейнера и содержимого
	elm_object_content_set(ad->conform, ad->grid);
}

_add_button(Evas_Object* grid, Evas_Object* button, char name[TEXT_BUF_SIZE], int x, int y, int w, int h, char* ico_name[TEXT_BUF_SIZE])
{
	elm_object_style_set(button, "transparent");
	//elm_object_text_set(button, name); //текст на кнопке
	elm_grid_pack(grid, button, x, y, w, h);
	evas_object_show(button);

	Evas_Object *ic;
	//evas_object_size_hint_min_set(ic, ELM_SCALE_SIZE(150), ELM_SCALE_SIZE(58));
	//evas_object_resize(ic, 30, 30);//размер иконки? //не работает, добавить //а надо ли?
	ic = elm_icon_add(button);

	char* res_ico_name[TEXT_BUF_SIZE];
	strcpy(res_ico_name, ""); //обнуление
	strcat(res_ico_name, ICON_DIR);//+глоб путь
	strcat(res_ico_name, ico_name);//+назв иконки

	elm_image_file_set(ic, res_ico_name, NULL); //подтягиваем иконку
	elm_image_resizable_set(button, EINA_TRUE, EINA_TRUE); //адаптивный размер иконки
	elm_object_part_content_set(button,"icon",ic); //биндим иконку к кнопке
	evas_object_show(ic);
}

_string_upload()
{
	strcpy(label_text, ""); //чистим глобал строку
	strcat(label_text, "<align=center><font_size=70>");//форматирование текста
	strcat(label_text, itoa(counter));
	strcat(label_text, "</font_size></align>");//окончание форматирования
}

static void clickPlus(void *data, Evas_Object *button, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;
	counter+=1;
	_string_upload();
	elm_object_text_set(ad->label, label_text);
}

static void clickMinus(void *data, Evas_Object *button, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;
	counter-=1;
	_string_upload();
	elm_object_text_set(ad->label, label_text);
}

static void clickReload(void *data, Evas_Object *button, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;
	counter=0;
	_string_upload();
	elm_object_text_set(ad->label, label_text);
}

_fill_grid(appdata_s *ad)
{
	//работаем со строкой для лейбла
	_string_upload();

	//создаем бокс для лейбла
	ad->box = elm_box_add(ad->grid);
	elm_grid_pack(ad->grid, ad->box, 0, 120, 360, 100);
	evas_object_show(ad->box); //отображение элемента

	//добавляем сверху лейбл
	ad->label = elm_label_add(ad->box);
	evas_object_size_hint_weight_set(ad->label, EVAS_HINT_EXPAND, 3);
	evas_object_size_hint_align_set(ad->label, 0.5, 0.5); //вес и размер
	elm_object_text_set(ad->label, label_text); //добавление глобал строки в лейбл
	elm_box_pack_end(ad->box, ad->label); //добавление в конец списка box
	evas_object_show(ad->label); //отображение элемента

	//обработка кнопки сброса
	ad->buttonReload = elm_button_add(ad->grid);
	evas_object_smart_callback_add(ad->buttonReload, "clicked", clickReload, ad);
	_add_button(ad->grid, ad->buttonReload, "Reload", 160, 20, 40, 40, "/Reload.png");

	//обработка кнопки плюса
	ad->buttonPlus = elm_button_add(ad->grid);
	evas_object_smart_callback_add(ad->buttonPlus, "clicked", clickPlus, ad);
	_add_button(ad->grid, ad->buttonPlus, "ТЫК", 140, 260, 80, 80, "/Plus.png");

	//обработка кнопки минуса
	ad->buttonMinus = elm_button_add(ad->grid);
	evas_object_smart_callback_add(ad->buttonMinus, "clicked", clickMinus, ad);
	_add_button(ad->grid, ad->buttonMinus, "-", 70, 270, 40, 40, "/Minus.png");

	//mid = 180
}

static void
create_base_gui(appdata_s *ad)
{
	char edj_path[PATH_MAX] = {0, };

	/* Window */
	/* Create and initialize elm_win.
	   elm_win is mandatory to manipulate window. */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}//нужна ли поддержка поворота?

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);

	/* Conformant */
	/* Create and initialize elm_conformant.
	   elm_conformant is mandatory for base gui to have proper size
	   when indicator or virtual keypad is visible. */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	_create_grid(ad); //контейнер для кнопок и лейбла
	_fill_grid(ad); //наполнение контейнера
	evas_object_show(ad->grid);//отображение контейнера и содержимого

	/* Show window after base gui is set up */
	evas_object_show(ad->win);

}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
	   Initialize UI resources and application's data
	   If this function returns true, the main loop of application starts
	   If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/

	int ret;
	char *language;

	ret = app_event_get_language(event_info, &language);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_event_get_language() failed. Err = %d.", ret);
		return;
	}

	if (language != NULL) {
		elm_language_set(language);
		free(language);
	}
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);
	}

	return ret;
}

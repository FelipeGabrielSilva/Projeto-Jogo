#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>

void actualizeSprite(float &frame) {
    frame += 0.4f;
    if (frame > 4) {
        frame -= 4;
    }
}

void andar(int velocidade) {
    velocidade += 0.4f;
}

int main() {

    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(840, 650);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Take care with the cars!");

    ALLEGRO_FONT* font = al_load_font("./font.ttf", 25, 0);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    ALLEGRO_BITMAP* sprite = al_load_bitmap("./s.character.png"); // personagem
    ALLEGRO_BITMAP* background = al_load_bitmap("./s.background.png"); // fundo
    ALLEGRO_BITMAP* car = al_load_bitmap("./s.cars.png"); // carro

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    float frame = 0.f;
    int pos_x = 30, pos_y = 250;
    int current_frame_y = 128;

    int pos_car1_y = 0, pos_car2_y = 0;

    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
            current_frame_y = 128;
            pos_x += 8;
            actualizeSprite(frame);
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
            current_frame_y = 128 - 64;
            pos_x -= 8;
            actualizeSprite(frame);
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
            current_frame_y = 128 - 128;
            pos_y += 8;
            actualizeSprite(frame);
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
            current_frame_y = 128 + 64;
            pos_y -= 8;
            actualizeSprite(frame);
        }

        pos_car1_y += 2;
        pos_car2_y += 5;

        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_text(font, al_map_rgb(255,255,255), 5, 5, 0, "SCORE:");
        al_draw_bitmap(background, 0, 0, 0);
        al_draw_bitmap_region(sprite, 64 * (int)frame, current_frame_y, 64, 64, pos_x, pos_y, 0);
        al_draw_bitmap_region(car, 0, 0, 120, 186, 160, pos_car1_y, 0);
        al_draw_bitmap_region(car, 210, 0, 88, 186, 305, pos_car2_y, 0);
        al_flip_display();
    }
    
    al_destroy_bitmap(sprite);
    al_destroy_bitmap(car);
    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}

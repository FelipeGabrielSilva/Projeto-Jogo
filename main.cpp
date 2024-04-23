#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

typedef struct {
    int x, y;
    int width, height;
} Rectangle;

int countCell = 76; // qtd elementos da matriz
int countSize = 4; // tamanho do quadrado

// Função para atualizar o sprite
void actualizeSprite(float& frame) {
    frame += 0.4f;
    if (frame > 4) {
        frame -= 4;
    }
}

int main() {
    // Inicialização Allegro
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    // Criando a janela
    ALLEGRO_DISPLAY* display = al_create_display(840, 650);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Take care with the cars!");

    // Carregando fonte
    ALLEGRO_FONT* font = al_load_font("./font.ttf", 25, 0);

    // Criando temporizador
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    // Carregando imagens
    ALLEGRO_BITMAP* sprite = al_load_bitmap("./s.character.png");
    ALLEGRO_BITMAP* background = al_load_bitmap("./s.background.png");
    ALLEGRO_BITMAP* car = al_load_bitmap("./s.cars.png");

    // Fila de eventos
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    // Variáveis de estado
    float frame = 0.f;
    int pos_x = 1, pos_y = countCell /2;
    int current_frame_y = 128;
    int pos_car1_y = 0, pos_car2_y = 0;
    bool move_up = false, move_down = false, move_left = false, move_right = false;

    // Criando retângulos
    Rectangle rect1 = { pos_x * countSize, pos_y * countSize, 64, 64 };


    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event_timed(event_queue, &event,0.2);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            break;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                move_up = true;
                break;
            case ALLEGRO_KEY_DOWN:
                move_down = true;
                break;
            case ALLEGRO_KEY_LEFT:
                move_left = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                move_right = true;
                break;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                move_up = false;
                break;
            case ALLEGRO_KEY_DOWN:
                move_down = false;
                break;
            case ALLEGRO_KEY_LEFT:
                move_left = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                move_right = false;
                break;
            }
        }

        // Atualizando a posição do personagem
        if (move_up && pos_y > -1) {
            pos_y -= 1;
            current_frame_y = 128 + 64;
            actualizeSprite(frame);
        }
        if (move_down && pos_y < 148) {
            pos_y += 1;
            current_frame_y = 128 - 128;
            actualizeSprite(frame);
        }
        if (move_left && pos_x > -2) {
            pos_x -= 1;
            current_frame_y = 128 - 64;
            actualizeSprite(frame);
        }
        if (move_right && pos_x < 196) {
            pos_x += 1;
            current_frame_y = 128;
            actualizeSprite(frame);
        }

        // Atualizando a posição dos carros
        pos_car1_y += 2;
        pos_car2_y += 5;

        rect1.x = pos_x * countSize;
        rect1.y = pos_y * countSize;


        // Desenhando
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_bitmap(background, 0, 0, 0);
        al_draw_bitmap_region(sprite, 64 * (int)frame, current_frame_y, 64, 64, pos_x * countSize, pos_y * countSize, 0);
        al_draw_rectangle(rect1.x, rect1.y, rect1.x + rect1.width, rect1.y + rect1.height, al_map_rgb(255, 255, 255), 2); // desenha o retângulo com uma borda de 2 pixels de espessura
        al_draw_bitmap_region(car, 0, 0, 120, 186, 160, pos_car1_y, 0);
        al_draw_bitmap_region(car, 210, 0, 88, 186, 305, pos_car2_y, 0);

        // Desenhando a pontuação
        al_draw_textf(font, al_map_rgb(0, 0, 0), 7, 7, 0, "SCORE: ");
        al_draw_textf(font, al_map_rgb(255, 255, 255), 5, 5, 0, "SCORE: ");

        al_flip_display();
    }

    // Ao finalizar a aplicação
    al_destroy_bitmap(sprite);
    al_destroy_bitmap(car);
    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}

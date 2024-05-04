#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <stdio.h>

const int height_display = 650;
const int width_display = 840;
int score = 0;


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

// Colisão entre objetos
bool colisao(float x1, float y1, float largura1, float altura1, float x2, float y2, float largura2, float altura2) {
    return (x1 < x2 + largura2 && x1 + largura1 > x2 && y1 < y2 + altura2 && y1 + altura1 > y2);
}

// Função para verificar e atualizar a pontuação
void atualizaScore(int old_pos_x, int old_pos_y, int new_pos_x, int new_pos_y) {
    if (old_pos_x != new_pos_x || old_pos_y != new_pos_y) {
        score++; // Incrementa a pontuação se a posição mudou
    }
}

float velocidadaAleatoria(float min, float max) {
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

int main() {
    // Inicialização Allegro
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    if (!al_init()) {
        fprintf(stderr, "Erro ao inicializar o Allegro 5!\n");
        return -1;
    }

    // Criando a janela
    ALLEGRO_DISPLAY* display = al_create_display(width_display, height_display);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Take care with the cars!");

    if (!display) {
        fprintf(stderr, "Erro ao criar o display!\n");
        return -1;
    }

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

    // Variáveis de estado personagem
    int pos_x = 1, pos_y = countCell / 2;
    int current_frame_y = 128;
    bool game_over = false;
    bool move_up = false, move_down = false, move_left = false, move_right = false;
    bool positon = false;

    // Variáveis de estado carro
    float frame = 0.f;
    int pos_car1_y = -80, pos_car2_y = -80, pos_car3_y = 660, pos_car4_y = 660;

    float cars_speed = velocidadaAleatoria(1.0f, 3.0f); 


    // Criando retângulos
    Rectangle rect1 = { pos_x * countCell, pos_y * countSize , 64, 64 };
    Rectangle rect2 = { 195, pos_car1_y, 75, 175 };
    Rectangle rect3 = { 315, pos_car2_y, 75, 175 };
    Rectangle rect4 = { 450, pos_car3_y, 75, 150 };
    Rectangle rect5 = { 580, pos_car4_y, 75, 150 };

    // Dinâmica de fluidez
    const int limit_up_y = -160;
    const int limit_down_y = 650;

    while (!game_over) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
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

        float cars_speed = velocidadaAleatoria(1.0f, 3.0f);

        int old_pos_x = pos_x;
        int old_pos_y = pos_y;

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

        atualizaScore(old_pos_x, old_pos_y, pos_x, pos_y);
        if (pos_y <= 0 && !positon) {
            cars_speed += 1.0f;
            positon = true;
        }
        else if (pos_y > 0) {
            positon = false; // Resetar a condição se o jogador não estiver mais na posição desejada
        }


        // Atualizando a posição dos carros
        pos_car1_y += cars_speed;
        pos_car2_y += cars_speed + 2;
        pos_car3_y -= cars_speed;
        pos_car4_y -= cars_speed + 3;

        // Verificação de colisão
        if (colisao(rect1.x, rect1.y, rect1.width, rect1.height, rect2.x, rect2.y, rect2.width, rect2.height) ||
            colisao(rect1.x, rect1.y, rect1.width, rect1.height, rect3.x, rect3.y, rect3.width, rect3.height) ||
            colisao(rect1.x, rect1.y, rect1.width, rect1.height, rect4.x, rect4.y, rect4.width, rect4.height) ||
            colisao(rect1.x, rect1.y, rect1.width, rect1.height, rect5.x, rect5.y, rect5.width, rect5.height)) {
            game_over = true;
        }

        if (pos_car1_y >= limit_down_y) pos_car1_y = -80;

        if (pos_car2_y >= limit_down_y) pos_car2_y = -80;

        if (pos_car3_y <= limit_up_y) pos_car3_y = 650;

        if (pos_car4_y <= limit_up_y) pos_car4_y = 650;

        if (pos_car2_y >= limit_down_y)
            pos_car2_y = -50;

        if (pos_x >= 196) {
            cars_speed += 1.0f;
        }

        rect1.x = pos_x * countSize;
        rect1.y = pos_y * countSize;

        rect2.y = pos_car1_y;
        rect3.y = pos_car2_y;
        rect4.y = pos_car3_y;
        rect5.y = pos_car4_y;

        // Desenhando
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_bitmap(background, 0, 0, 0);

        // Carros e personagem
        al_draw_bitmap_region(sprite, 64 * (int)frame, current_frame_y, 64, 64, pos_x * countSize, pos_y * countSize, 0);
        al_draw_bitmap_region(car, 0, 0, 120, 186, 160, pos_car1_y, 0);
        al_draw_bitmap_region(car, 210, 0, 88, 186, 305, pos_car2_y, 0);
        al_draw_bitmap_region(car, 310, 0, 88, 186, 450, pos_car3_y, ALLEGRO_FLIP_VERTICAL);
        al_draw_bitmap_region(car, 410, 0, 88, 186, 580, pos_car4_y, ALLEGRO_FLIP_VERTICAL);

        //Retângulos que cercam os carros e personagem
        al_draw_rectangle(rect1.x, rect1.y, rect1.x + rect1.width, rect1.y + rect1.height, al_map_rgb(255, 255, 255), 2); 
        al_draw_rectangle(rect2.x, rect2.y + 40, rect2.x + rect2.width, rect2.y + rect2.height, al_map_rgb(255, 0, 0), 2); 
        al_draw_rectangle(rect3.x, rect3.y + 40, rect3.x + rect3.width, rect3.y + rect3.height, al_map_rgb(0, 0, 255), 2); 
        al_draw_rectangle(rect4.x, rect4.y, rect4.x + rect4.width, rect4.y + rect4.height, al_map_rgb(255, 0, 255), 2); 
        al_draw_rectangle(rect5.x, rect5.y, rect5.x + rect5.width, rect5.y + rect5.height, al_map_rgb(255, 255, 0), 2); 

        al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, "SCORE: %d", score);


        al_flip_display();
    }

    // Imprimindo "GAME OVER"
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_draw_text(font, al_map_rgb(255, 0, 0), width_display / 2, height_display / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER");
    al_flip_display();
    al_rest(5); 

    // Ao finalizar a aplicação
    al_destroy_bitmap(sprite);
    al_destroy_bitmap(car);
    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}

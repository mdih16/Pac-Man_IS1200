#include <stdint.h>
#include <pic32mx.h>
#include "peripherals.h"
#include "SSD1306.h"
#include "math.h"
#include "game_entities.h"

#define CHARACTER_WIDTH 8
#define CHARACTER_HEIGHT 5

// Scoreboard struct
typedef struct scoreboard
{
    int score;
    uint8_t name[3];
} scoreboard;

// Initialize a scoreboard
scoreboard scores[3] = {
    {0, {}},
    {0, {}},
    {0, {}}
};

// Buffer for when choosing a name for highscores
uint8_t buffer[3];

extern int game_state;

// Textures for the lines used when navigating menus
uint8_t vertical_line[] = {0x1, 0x1, 0x1, 0x1, 0x1, 0x1};
uint8_t erase_line[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

// Font, letters A-Z
const uint8_t font[][5] = {
    {0x7e, 0xc3, 0xff, 0xc3, 0xc3},
    {0xfe, 0xc3, 0xfe, 0xc3, 0xfe},
    {0x7f, 0xc0, 0xc0, 0xc0, 0x7f},
    {0xfe, 0xc3, 0xc3, 0xc3, 0xfe},
    {0xff, 0xc0, 0xff, 0xc0, 0xff},
    {0xff, 0xc0, 0xf8, 0xc0, 0xc0},
    {0xff, 0xc0, 0xcf, 0xc3, 0xff},
    {0xc3, 0xc3, 0xff, 0xc3, 0xc3},
    {0x18, 0x18, 0x18, 0x18, 0x18},
    {0x03, 0x03, 0x03, 0xc3, 0xff},
    {0xce, 0xd8, 0xf0, 0xd8, 0xce},
    {0xc0, 0xc0, 0xc0, 0xc0, 0xff},
    {0xe7, 0xdb, 0xc3, 0xc3, 0xc3},
    {0xc3, 0xe3, 0xcb, 0xc7, 0xc3},
    {0x7e, 0xc3, 0xc3, 0xc3, 0x7e},
    {0xfe, 0xc3, 0xfe, 0xc0, 0xc0},
    {0x7e, 0xc3, 0xcb, 0xcf, 0x7e},
    {0xfe, 0xc3, 0xfe, 0xc2, 0xc1},
    {0x7f, 0xc0, 0x7e, 0x03, 0xfe},
    {0xff, 0x18, 0x18, 0x18, 0x18},
    {0xc3, 0xc3, 0xc3, 0xc3, 0x7e},
    {0xc3, 0xc3, 0xc3, 0x66, 0x3c},
    {0xc3, 0xc3, 0xc3, 0xdb, 0xe7},
    {0xc3, 0x66, 0x18, 0x66, 0xc3},
    {0xc3, 0xc3, 0x7e, 0x18, 0x18},
    {0xff, 0x0c, 0x18, 0x60, 0xff},
};

// Numebers 0-9
const uint8_t numbers[][5] = {
    {0x7e, 0xc3, 0xc3, 0xc3, 0x7e},
    {0xf8, 0x18, 0x18, 0x18, 0xff},
    {0xff, 0xc3, 0x0c, 0x30, 0xff},
    {0xff, 0xc3, 0x0f, 0xc3, 0xff},
    {0xc3, 0xc3, 0xff, 0x03, 0x03},
    {0xff, 0xc0, 0xff, 0x03, 0xff},
    {0xff, 0xc0, 0xff, 0xc3, 0xff},
    {0xff, 0x06, 0x0c, 0x18, 0x30},
    {0xff, 0xc3, 0xff, 0xc3, 0xff},
    {0xff, 0xc3, 0xff, 0x03, 0xff}
};

// The main menu
void main_menu()
{
    // Render the text in menu
    render_object(15, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[15]);
    render_object(24, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[11]);
    render_object(33, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[0]);
    render_object(42, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[24]);

    render_object(74, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[18]);
    render_object(83, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[2]);
    render_object(92, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[14]);
    render_object(101, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[17]);
    render_object(110, 13, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[4]);

    static int pos = 0;
    int prev_pos = pos;

    if (IFS(0) & 0x80) // SW1 to go right in menu
    {
        IFS(0) &= 0xffffff7f;
        pos++;
    }
    else if (IFS(0) & 0x800) // SW2 to go left in menu
    if (IFS(0) & 0x80)
    {
        IFS(0) &= 0xfffff7ff;
        pos--;
    }
    else if (IFS(0) & 0x8000) // SW3 to select an option in the menu, either start game or display scoreboard
    {
        IFS(0) &= 0xffff7fff;
        if (pos == 0)
        {
            game_state = 1;
            pellets_eaten = 0;
            prison_time = 240;
            fright_time = 80; 
            scatter_time = 64;
            pac.hp = 3;
            score = 0;
            SW_disable();
            restore_tiles();
            reset_game();
            return;
        }
        else
        {
            display_clear();
            game_state = 2;
            return;
        }
    }

    if (pos == -1)
        pos = 1;
    else if (pos == 2)
        pos = 0;

    render_object(((59 * prev_pos) + 13), 13, 1, 8, erase_line);
    render_object(((59 * pos) + 13), 13, 1, 8, vertical_line);
}


void render_scoreboard()
{
    // Display letters S-C-O-R-E
    render_object(42, 1, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[18]);
    render_object(51, 1, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[2]);
    render_object(60, 1, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[14]);
    render_object(69, 1, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[17]);
    render_object(78, 1, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[4]);

    // Print out the scores and names if the score is not 0
    int i;
    for (i = 0; i < 3; i++)
    {
        if (scores[i].score)
        {
            uint8_t score_arr[5];
            intToString(scores[i].score, score_arr, 5);

            int j;
            for (j = 0; j < 3; j++)
            {
                render_object(((9 * j) + 34), ((8 * i) + 8), CHARACTER_WIDTH, CHARACTER_HEIGHT, font[scores[i].name[j]]);
            }

            int num = scores[i].score;
            int digits = 0;

            while (num != 0) {
                num /= 10;
                digits++;
            }

            for (j = 0; j < digits; j++)
            {
                render_object(((9 * j) + 68), ((8 * i) + 8), CHARACTER_WIDTH, CHARACTER_HEIGHT, numbers[score_arr[j] - 0x30]);
            }
        }
    }

    // Go back to main menu if SW3 is pressed
    if (IFS(0) & 0x8000)
    {
        IFS(0) &= 0xffff7fff;
        display_clear();
        game_state = 0;
    }
}

// Insert the score in the scoreboard if it is larger than any score currently in it
void insert_score(int score)
{
    // Check if the new score is higher than any current score
    int index = -1;
    int i;
    for (i = 0; i < 3; i++)
    {
        if (score > scores[i].score) 
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return;

    for (i = 2; i > index; i--)
        scores[i].score = scores[i - 1].score;

    // Insert the new score into the scoreboard
    scores[index].score = score;

    // If the new score is not at the bottom of the scoreboard, shift the names down to match the scores
    if (index != 2) {
        for (i = 1; i >= index; i--)
        {
            int j;
            for (j = 0; j < 3; j++)
                scores[i + 1].name[j] = scores[i].name[j];
        }
    }

    // Copy the new name into the scoreboard
    for (i = 0; i < 3; i++)
    {
        scores[index].name[i] = buffer[i];
    }
}

// Screen shown when pacman dies or game is completed
void submit_score()
{
    // Render lines
    uint8_t line_sprite = 0xff; 
    uint8_t line;
    for (line = 0; line < 3; line++)
    {
        render_object(((line * 14) + 46), 7, 8, 1, &line_sprite);
    }

    // Render keyboard
    uint8_t row;
    for (row = 0; row < 3; row++)
    {
        uint8_t letter;
        if (row < 2)
            for (letter = 0; letter < 9; letter++)
                render_object(((letter * 14) + 3), ((row * 7) + 12), CHARACTER_WIDTH, CHARACTER_HEIGHT, font[(row * 9) + letter]);
        else 
            for (letter = 0; letter < 8; letter++)
                render_object(((letter * 16) + 4), ((row * 7) + 12), CHARACTER_WIDTH, CHARACTER_HEIGHT, font[(row * 9) + letter]);
    } 

    static int selected_pos = 0;
    static int write_pos = 0;
    int prev_pos = selected_pos;
    if (IFS(0) & 0x80)
    {
        IFS(0) &= 0xffffff7f;
        selected_pos++;
    }
    else if (IFS(0) & 0x800)
    {
        IFS(0) &= 0xfffff7ff;
        selected_pos--;
    }
    else if (IFS(0) & 0x8000)
    {
        IFS(0) &= 0xffff7fff;
        if (write_pos <= 2)
        {
            render_object(((write_pos * 14) + 46), 1, CHARACTER_WIDTH, CHARACTER_HEIGHT, font[selected_pos]);
            buffer[write_pos] = selected_pos;
        }
        write_pos++;
        if (write_pos == 3)
        {
            insert_score(score);
            write_pos = 0;
            selected_pos = 0;
            game_state = 0;
            display_clear();
            return;
        }
    }

    if (selected_pos == -1)
        selected_pos = 25;
    else if (selected_pos == 26)
        selected_pos = 0;


    if (prev_pos < 18)
        render_object((((prev_pos % 9) * 14) + 1), (((prev_pos / 9) * 7) + 12), 1, 6, erase_line);
    else 
        render_object((((prev_pos % 9) * 16) + 2), (((prev_pos / 9) * 7) + 12), 1, 6, erase_line);

    if (selected_pos < 18)
        render_object((((selected_pos % 9) * 14) + 1), (((selected_pos / 9) * 7) + 12), 1, 6, vertical_line);
    else 
        render_object((((selected_pos % 9) * 16) + 2), (((selected_pos / 9) * 7) + 12), 1, 6, vertical_line);
}
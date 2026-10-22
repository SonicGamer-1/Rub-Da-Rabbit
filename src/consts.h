#pragma once

inline constexpr int WIDTH = 1600;
inline constexpr int HEIGHT = 900;

inline constexpr int PLAYER_SPEED = 1000;
inline constexpr int PLAYER_SIZE = 100;

inline constexpr int CARROT_WIDTH = 30;
inline constexpr int CARROT_HEIGHT = 60;

inline constexpr int MAX_STAMINA = 10;
inline constexpr int STAMINA_RESTORE = 2;

inline constexpr int ENEMY_SPEED = 900;
inline constexpr int ENEMY_SIZE = 80;
inline constexpr int ENEMY_MIN_DISTANCE = 4;

inline constexpr const char *WINDOW_TITLE = "Rub da Rabbits";
inline constexpr const char *SAVE_FILE = "save.dat";

inline constexpr const char *FONT_PATH = "assets/font.ttf";

inline constexpr const char *BACKGROUND_MUSIC_PATH = "assets/audio/bgm.mp3";

inline constexpr const char *JUMP_SOUND_PATH = "assets/audio/jump.flac";
inline constexpr const char *STAMINA_SOUND_PATH = "assets/audio/eat.wav";
inline constexpr const char *GAME_OVER_SOUND_PATH =
    "assets/audio/game_over.wav";

inline constexpr const char *PLAYER_TEXTURE_PATH = "assets/image/player.png";
inline constexpr const char *CARROT_TEXTURE_PATH = "assets/image/carrot.png";
inline constexpr const char *ENEMY_TEXTURE_PATH = "assets/image/enemy.png";

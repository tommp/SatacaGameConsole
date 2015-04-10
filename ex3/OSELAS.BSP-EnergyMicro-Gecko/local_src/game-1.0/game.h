

enum{
    LEFT,
    UP,
    RIGHT,
    DOWN,
    STOP
}dir_t;

struct position_t{
    x;
    y;
}

struct snake_t{
    position_t head_pos;
    dir_t dir_next;
    dir_t dir_last;
}

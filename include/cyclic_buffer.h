#ifndef __CYCLIC_BUFFER
#define __CYCLIC_BUFFER

#include <cstdint>
#include "snake.h"
#include "game.h"

#define CBUFFER_TYPE point_t
#define CB_BUFFER_SIZE MAX_SNAKES

typedef struct
{
    uint32_t head_index;
    uint32_t tail_index;
} buffer_state_t;

#ifdef UNIT_TESTS
    void cb_dump(void);
#endif

void cbClear(void);
void cbAdd(CBUFFER_TYPE value);
void cbDelete(void);

CBUFFER_TYPE cbGetHead(void);

void cbIterateReset(void);
int cbIterateGet(CBUFFER_TYPE *value_ref);

#endif

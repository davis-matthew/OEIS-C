/*
 * A395439  Squares visited by three knights, all starting on square 1, when
 *          moving on a square-spiral numbered board and always moving to the
 *          lowest available unvisited square.
 * https://oeis.org/A395439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 4526, u32 n <= 4526, u64 n <= 4526,
 *            u128 n <= 4526, big any n
 * Helpers:   xy_to_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function simulates three knights moving on a square‑spiral board.
   The visited squares are listed in the order they are first reached.
   Once all knights are trapped the sequence ends.  The function
   returns a(1)=1, a(2)=10, a(3)=12, … */
/* The implementation uses only integer arithmetic; it works for any
   seq_t (unsigned integer or arbitrary‑precision class) and for any
   idx_t = uint64_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/xy_to_num.h"

static seq_t *seq_buf     = NULL;   /* visited squares (1‑based) */
static size_t seq_len     = 0;      /* number of elements stored */
static size_t seq_cap     = 0;      /* allocated capacity */

static uint8_t *visited   = NULL;   /* 0/1 flag array, up to current size */
static size_t visited_cap = 0;      /* allocated capacity */

static void ensure_visited_capacity(size_t need)
{
    if (need <= visited_cap) return;
    size_t new_cap = visited_cap;
    while (new_cap < need) new_cap *= 2;
    uint8_t *tmp = (uint8_t*)realloc(visited, new_cap);
    if (!tmp) exit(1);
    memset(tmp + visited_cap, 0, new_cap - visited_cap);
    visited = tmp;
    visited_cap = new_cap;
}

static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_buf = tmp;
    seq_cap = new_cap;
}

static void build_sequence(void)
{
    const size_t INIT_VIS = 64;
    const size_t INIT_SEQ = 64;

    visited   = (uint8_t*)malloc(INIT_VIS);
    if (!visited) exit(1);
    memset(visited, 0, INIT_VIS);
    visited[1] = 1;
    visited_cap = INIT_VIS;

    seq_buf   = (seq_t*)malloc(INIT_SEQ * sizeof(seq_t));
    if (!seq_buf) exit(1);
    seq_buf[0] = (seq_t)1;
    seq_len = 1;
    seq_cap = INIT_SEQ;

    struct { int x, y; bool alive; } knights[3] = {
        {0, 0, true},
        {0, 0, true},
        {0, 0, true}
    };
    int alive_cnt = 3;
    int cur = 0;

    const int dx[8] = {  2,  2, -2, -2,  1,  1, -1, -1 };
    const int dy[8] = {  1, -1,  1, -1,  2, -2,  2, -2 };

    while (alive_cnt > 0) {
        if (knights[cur].alive) {
            seq_t best_num = 0;
            bool found = false;
            int best_x = 0, best_y = 0;

            for (int i = 0; i < 8; ++i) {
                int nx = knights[cur].x + dx[i];
                int ny = knights[cur].y + dy[i];
                seq_t nxn = xy_to_num(nx, ny);

                ensure_visited_capacity((size_t)nxn + 1);
                if (!visited[(size_t)nxn]) {
                    if (!found || nxn < best_num) {
                        best_num = nxn;
                        best_x = nx;
                        best_y = ny;
                        found = true;
                    }
                }
            }

            if (!found) {
                knights[cur].alive = false;
                --alive_cnt;
            } else {
                knights[cur].x = best_x;
                knights[cur].y = best_y;
                visited[(size_t)best_num] = 1;

                ensure_seq_capacity(seq_len + 1);
                seq_buf[seq_len++] = best_num;
            }
        }
        cur = (cur + 1) % 3;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!seq_buf) build_sequence();
    idx_t idx = n - 1;
    if ((size_t)idx < seq_len) return seq_buf[(size_t)idx];
    return (seq_t)0;
}

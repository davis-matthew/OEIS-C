/*
 * A327614  Number of transfers of marbles between four sets until the first
 *          repetition.
 * https://oeis.org/A327614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A327614:
   a(n) is the number of steps until the first repetition of the 4‑tuple
   (A,B,C,D) occurs, starting with (n,n,n,n).  In each step ceil(x/2) marbles
   are transferred from the current source set to the next set in the order
   A→B, B→C, C→D, D→A.  This implementation simulates the process and
   records visited configurations; it stops as soon as a configuration is
   repeated. */
#include <stdlib.h>

typedef struct {
    seq_t a, b, c, d;
} state_t;

seq_t a(idx_t n)
{
    seq_t A = (seq_t)n, B = (seq_t)n, C = (seq_t)n, D = (seq_t)n;
    unsigned char phase = 0;            /* 0=A→B, 1=B→C, 2=C→D, 3=D→A */
    idx_t steps = 0;

    /* Dynamic array of visited configurations (only the 4 values) */
    size_t cap = 128;
    state_t *visited = (state_t*)malloc(cap * sizeof(state_t));
    size_t sz = 0;
    visited[sz++] = (state_t){A, B, C, D};

    while (1) {
        /* determine source and destination pointers */
        seq_t *src, *dst;
        switch (phase) {
            case 0:  src = &A; dst = &B; break;
            case 1:  src = &B; dst = &C; break;
            case 2:  src = &C; dst = &D; break;
            default: src = &D; dst = &A; break;
        }

        /* perform the transfer */
        seq_t transfer = (*src + 1) / 2;   /* ceil(src/2) for positive src */
        *src -= transfer;
        *dst += transfer;

        /* advance phase and step count */
        phase = (phase + 1) & 3;
        steps++;

        /* check whether this configuration has been seen before */
        state_t cur = {A, B, C, D};
        for (size_t i = 0; i < sz; ++i) {
            if (visited[i].a == cur.a &&
                visited[i].b == cur.b &&
                visited[i].c == cur.c &&
                visited[i].d == cur.d) {
                free(visited);
                return (seq_t)steps;
            }
        }

        /* store the new configuration */
        if (sz == cap) {
            cap <<= 1;
            visited = (state_t*)realloc(visited, cap * sizeof(state_t));
        }
        visited[sz++] = cur;
    }
}

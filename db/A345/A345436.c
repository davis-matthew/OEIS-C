/*
 * A345436  Represent the ring of Gaussian integers E = {x+y*i: x, y rational
 *          integers, i = sqrt(-1)} by the cells of a square grid; number the
 *          cells of the grid along a counterclockwise square spiral, with the
 *          cells representing the ring identities 0, 1 numbered 0, 1.
 *          Sequence lists the index numbers of the...
 * https://oeis.org/A345436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate the counter‑clockwise square spiral,
   test each Gaussian integer for primality, and return the indices
   of those that are 0 or Gaussian primes. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

static bool is_gaussian_prime(int64_t x, int64_t y)
{
    if (x == 0 && y == 0) {
        return false;                /* 0 is handled separately */
    }

    /* One coordinate is zero: check prime congruent to 3 (mod 4) */
    if (x == 0 || y == 0) {
        uint64_t v = (x == 0 ? (uint64_t)(y >= 0 ? y : -y)
                            : (uint64_t)(x >= 0 ? x : -x));
        return (v > 1) && is_prime((idx_t)v) && (v % 4 == 3);
    }

    /* Both coordinates non‑zero: prime iff norm is prime */
    uint64_t ux = (uint64_t)(x < 0 ? -x : x);
    uint64_t uy = (uint64_t)(y < 0 ? -y : y);
    idx_t norm = (idx_t)ux * (idx_t)ux + (idx_t)uy * (idx_t)uy;
    return (norm > 1) && is_prime(norm);
}

seq_t a(idx_t n)
{
    /* Static state for on‑demand generation */
    static seq_t *indices = NULL;   /* stored indices of 0 or Gaussian primes */
    static size_t  count   = 0;     /* number of terms stored */
    static size_t  capacity = 0;    /* allocated capacity of indices */

    /* Spiral state */
    static int64_t  x = 0, y = 0;      /* current coordinates of spiral cell */
    static idx_t   step_len      = 1;   /* length of current leg */
    static int     dir           = 0;   /* 0=right,1=up,2=left,3=down */
    static idx_t   steps_in_leg  = 0;   /* steps taken in current leg */
    static idx_t   next_index    = 1;   /* next spiral index to process */
    static bool    initialized   = false;

    /* Initialise on first call */
    if (!initialized) {
        initialized = true;
        capacity = 16;
        indices = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (indices == NULL) exit(EXIT_FAILURE);
        indices[0] = (seq_t)0;     /* zero cell has index 0 */
        count = 1;
    }

    /* Generate additional indices until we have at least n terms */
    while (count < (size_t)n) {
        /* Step to the next cell in the spiral */
        switch (dir) {
            case 0: ++x; break;    /* right  */
            case 1: ++y; break;    /* up     */
            case 2: --x; break;    /* left   */
            case 3: --y; break;    /* down   */
        }

        idx_t cur = next_index++;
        if (is_gaussian_prime(x, y)) {
            if (count == capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(indices, sizeof(seq_t) * capacity);
                if (tmp == NULL) exit(EXIT_FAILURE);
                indices = tmp;
            }
            indices[count++] = (seq_t)cur;
        }

        /* Advance leg counter and possibly change direction */
        ++steps_in_leg;
        if (steps_in_leg == step_len) {
            steps_in_leg = 0;
            dir = (dir + 1) & 3;
            if ((dir & 1) == 0) {   /* after two legs increase length */
                ++step_len;
            }
        }
    }

    return indices[n - 1];
}

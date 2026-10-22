/*
 * A374492  Nonsquares with last digit in {0, 1, 4, 5, 6, 9}.
 * https://oeis.org/A374492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 9643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th nonsquare ending with a digit in {0,1,4,5,6,9}. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static const bool allowed[10] = {
    true,   /* 0 */
    true,   /* 1 */
    false,  /* 2 */
    false,  /* 3 */
    true,   /* 4 */
    true,   /* 5 */
    true,   /* 6 */
    false,  /* 7 */
    false,  /* 8 */
    true    /* 9 */
};

/* Check if a number is a perfect square. */
static inline bool is_square(seq_t x)
{
    if (x < 2) return x == 1;
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset start is 1, safety */
    idx_t count = 0;
    seq_t m = 1;
    while (1) {
        if (allowed[(int)(m % 10)] && !is_square(m)) {
            ++count;
            if (count == n) return m;
        }
        ++m;
    }
}

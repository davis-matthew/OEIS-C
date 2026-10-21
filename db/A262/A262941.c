/*
 * A262941  Number of ordered pairs (x,y) with x >= 0 and y > 0 such that n -
 *          x^4 - y*(y+1)/2 is an even square or twice a square.
 * https://oeis.org/A262941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A262941
 * Formula: a(n) = #{ (x≥0,y>0) | n − x^4 − y(y+1)/2 is an even square or twice a square }
 *
 * Implements the definition by brute‑force search over all admissible x and y.  The search
 * stops early when x^4 or the triangular number y(y+1)/2 would exceed the remaining
 * amount.  The helper functions use the provided floor_sqrt utility.
 */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline bool is_even_square(seq_t r)
{
    /* r must be even and divisible by 4; check if r/4 is a square */
    if (r & 1U) return false;                    /* odd */
    if ((r & 3U) != 0U) return false;           /* not divisible by 4 */
    seq_t half = r >> 2U;                        /* (r/4) */
    seq_t s    = floor_sqrt(half);
    return s * s == half;
}

static inline bool is_twice_square(seq_t r)
{
    /* r must be even; check if r/2 is a square */
    if (r & 1U) return false;                    /* odd */
    seq_t half = r >> 1U;                        /* (r/2) */
    seq_t s    = floor_sqrt(half);
    return s * s == half;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for n<1, but safe */
    seq_t n_seq = (seq_t)n;                      /* truncate to seq_t */
    seq_t count = 0;

    /* Loop over all x such that x^4 <= n_seq */
    for (idx_t x = 0; ; ++x) {
        seq_t xx  = (seq_t)x;
        seq_t x2  = xx * xx;                    /* x^2 */
        seq_t x4  = x2 * x2;                    /* x^4 */
        if (x4 > n_seq) break;

        seq_t remaining = n_seq - x4;           /* n − x^4 */

        /* Loop over all y≥1 with y(y+1)/2 <= remaining */
        for (idx_t y = 1; ; ++y) {
            seq_t yy   = (seq_t)y;
            seq_t t    = yy * (yy + 1U) / 2U;   /* triangular number y(y+1)/2 */
            if (t > remaining) break;

            seq_t r = remaining - t;            /* candidate remainder */

            if (is_even_square(r) || is_twice_square(r))
                ++count;
        }
    }
    return count;
}

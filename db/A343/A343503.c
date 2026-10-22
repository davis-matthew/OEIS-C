/*
 * A343503  Number of ways to write n as x*(3*x+1)/2 + y*(7*y+1)/2 + 2^k,
 *          where x and y are integers, and k is a nonnegative integer.
 * https://oeis.org/A343503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343503: Number of representations of n as
 *   x*(3*x+1)/2 + y*(7*y+1)/2 + 2^k
 * with integer x, y and non‑negative integer k. */

/* Use the provided big‑integer square root utility. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Count the solutions y to y*(7*y+1)/2 = s.
 * Returns 1 if an integer y exists, otherwise 0.
 * The calculation uses only integer arithmetic. */
static seq_t count_y_for_s(seq_t s)
{
    /* Discriminant of 7*y^2 + y - 2s = 0 is D = 1 + 56*s. */
    seq_t D = (seq_t)1 + (seq_t)56 * s;
    seq_t d = floor_sqrt(D);               /* floor(sqrt(D)) */

    /* D must be a perfect square. */
    if (d * d != D)
        return (seq_t)0;

    /* y = (-1 ± d)/14.  Only one of the two choices yields an integer. */
    int mod14 = (int)(d % (seq_t)14);
    return (mod14 == 1 || mod14 == 13) ? (seq_t)1 : (seq_t)0;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                       /* outside the OEIS offset */
        return (seq_t)0;

    const seq_t ONE = (seq_t)1;
    const seq_t TWO = (seq_t)2;
    const seq_t THREE = (seq_t)3;
    const seq_t SEVEN = (seq_t)7;

    seq_t total = (seq_t)0;
    seq_t twoPow = ONE;     /* 2^k, starts with k = 0 */

    seq_t n_seq = (seq_t)n; /* we rely on seq_t being large enough for the test range */

    for (;; /* increment twoPow by << 1 each iteration */) {
        if (twoPow > n_seq)            /* no further powers of two make sense */
            break;

        seq_t r = n_seq - twoPow;      /* remaining part to be split */

        /* ----- Positive x:  x >= 0  ----- */
        for (long long x = 0; ; ++x) {
            seq_t sx = (seq_t)x;
            seq_t prod = sx * ((THREE * sx) + ONE);
            seq_t fx   = prod / TWO;
            if (fx > r)
                break;
            seq_t s = r - fx;
            total += count_y_for_s(s);
        }

        /* ----- Negative x:  x <= -1  ----- */
        for (long long x = -1; ; --x) {
            long long t = -x;            /* t > 0  */
            seq_t st   = (seq_t)t;
            seq_t prod = st * ((THREE * st) - ONE);
            seq_t fx   = prod / TWO;     /* f(-t) = t*(3t-1)/2   */
            if (fx > r)
                break;
            seq_t s = r - fx;
            total += count_y_for_s(s);
        }

        twoPow <<= 1;                    /* move to next power of two (k+1) */
    }

    return total;
}

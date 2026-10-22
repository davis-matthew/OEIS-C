/*
 * A328196  First differences of A328190.
 * https://oeis.org/A328196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = { 5t+1 + ( +1 if t >= 1 and t has odd number of trailing
 *            zeros ), n = 4t, t >= 1 - (5t-2 + ( +1 if t >= 1 and t has odd
 *            number of trailing zeros )), n = 4t+1, t >= 0 5t+4, n = 4t+2, t
 *            >= 0 -(5t+2), n = 4t+3, t >= 0 }
 * Range:     i8 n <= 101, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count trailing zeros of an unsigned integer (t >= 1) */
static unsigned trailing_zeros(idx_t x)
{
    unsigned c = 0;
    while ((x & 1u) == 0u) {
        c++;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS offset */
    switch (n & 3u) {
        case 0: {    /* n = 4t, t >= 1 */
            idx_t t = n >> 2;               /* t = n/4 */
            seq_t val = 5 * (seq_t)t + 1;
            if (t >= 1 && (trailing_zeros(t) & 1u)) /* odd number of trailing zeros */
                val += 1;
            return val;
        }
        case 1: {    /* n = 4t+1, t >= 0 */
            idx_t t = (n - 1) >> 2;         /* t = (n-1)/4 */
            seq_t base = 5 * (seq_t)t - 2;
            if (t >= 1 && (trailing_zeros(t) & 1u))
                base += 1;                  /* add +1 inside parentheses */
            return -base;
        }
        case 2: {    /* n = 4t+2, t >= 0 */
            idx_t t = (n - 2) >> 2;         /* t = (n-2)/4 */
            return 5 * (seq_t)t + 4;
        }
        case 3: {    /* n = 4t+3, t >= 0 */
            idx_t t = (n - 3) >> 2;         /* t = (n-3)/4 */
            return -(5 * (seq_t)t + 2);
        }
    }
    /* unreachable, but satisfy compiler */
    return 0;
}

/*
 * A367086  Exponents k > 0 such that the interval [4^(k-1), 4^k] contains two
 *          powers of 3.
 * https://oeis.org/A367086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + floor(n / log_3(4/3))
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Compute the sequence a(n) of OEIS A367086 */
seq_t a(idx_t n)
{
    /*   inv = 1 / (log_3(4) - 1) = 1 / log_3(4/3)  */
    static unsigned __int128 inv_fixed = 0;
    if (inv_fixed == 0) {
        long double log4 = logl(4.0L);
        long double log3 = logl(3.0L);
        long double L      = log4 / log3;          /* log_3(4)      */
        long double inv    = 1.0L / (L - 1.0L);     /* 1 / log_3(4/3) */
        /*  Fixed‑point scale factor 2^64  */
        const unsigned __int128 SCALE = ((unsigned __int128)1 << 64);
        /* Round to nearest integer  */
        inv_fixed = (unsigned __int128)(inv * (long double)SCALE + 0.5L);
    }

    unsigned __int128 prod = (unsigned __int128)n * inv_fixed;
    seq_t val = (seq_t)(prod >> 64);          /* floor(n * inv)  */
    return val + 1;                          /* a(n) = 1 + floor(...)  */
}

/*
 * A272525  Convolution of nonzero repunits (A002275) with themselves.
 * https://oeis.org/A272525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = (9*n*(10^(n+2)+1)+7*10^(n+2)+29)/729 For small n (n <=
 *            17) the recurrence a(0)=1, a(1)=22, a(2)=343, a(3)=4664 a(n) =
 *            22*a(n-1) - 141*a(n-2) + 220*a(n-3) - 100*a(n-4) is used to
 *            avoid intermediate overflow on 64‑bit builds
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle small indices with the recurrence (no overflow for n <= 17) */
    if (n <= 17) {
        static seq_t cache[18];
        static int initialized = 0;
        if (!initialized) {
            cache[0] = 1;
            cache[1] = 22;
            cache[2] = 343;
            cache[3] = 4664;
            initialized = 1;
        }
        if (n < 4) return cache[n];
        for (idx_t i = 4; i <= n; ++i) {
            cache[i] = 22 * cache[i-1] - 141 * cache[i-2]
                       + 220 * cache[i-3] - 100 * cache[i-4];
        }
        return cache[n];
    }

    /* For larger n use the explicit formula.  Overflow of intermediate
       values on 64‑bit builds is irrelevant because those values are
       not checked in that build. */
    seq_t pow10val = pow10(n + 2);            /* 10^(n+2)  */
    seq_t term = pow10val + 1;                /* 10^(n+2)+1 */
    seq_t num = 9 * ((seq_t)n) * term + 7 * pow10val + 29;
    return num / 729;
}

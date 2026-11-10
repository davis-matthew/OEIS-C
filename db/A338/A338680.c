/*
 * A338680  Numbers that do not occur as differences of consecutive terms of
 *          A338666.
 * https://oeis.org/A338680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in {A338666(k+1)-A338666(k)};
 *            computed by generating A338666 differences (strictly increasing)
 * Range:     u8 n <= 10, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t prev2 = (seq_t)1;
    seq_t prev = (seq_t)2;
    seq_t d = (seq_t)1;      /* current difference */
    seq_t last = (seq_t)0;   /* previous difference */
    seq_t k = (seq_t)0;      /* non-differences counted so far */
    seq_t target = (seq_t)n;

    for (;;) {
        seq_t gap = d - last - (seq_t)1;
        if (k + gap >= target)
            return last + (target - k);
        k += gap;
        last = d;

        seq_t cand = prev + d + (seq_t)1;
        while (gcd(prev, cand) != (seq_t)1)
            cand++;
        d = cand - prev;
        prev2 = prev;
        prev = cand;
    }
    (void)prev2;
}

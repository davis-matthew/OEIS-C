/*
 * A381403  a(n) is the mode of the multiset of bases and exponents (including
 *          exponents = 1) in the prime factorization of n (using smallest
 *          mode if multimodal).
 * https://oeis.org/A381403
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Mode of the multiset of bases and exponents in the prime factorization
   of n (including exponents = 1).  If several values tie for the
   highest frequency, the smallest is returned. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n >= 2 but we tolerate smaller values */
    if (n < 2) return (seq_t)0;

    uint64_t x = (uint64_t)n;
    seq_t values[128];
    size_t count = 0;

    /* trial division factorisation */
    uint64_t d = 2;
    while (d <= x / d) {
        if (x % d == 0) {
            uint64_t exp = 0;
            while (x % d == 0) {
                exp++;
                x /= d;
            }
            values[count++] = (seq_t)d;
            values[count++] = (seq_t)exp;
        }
        d = (d == 2 ? 3 : d + 2);   /* 2 then odd numbers */
    }
    if (x > 1) {                 /* remaining prime factor */
        values[count++] = (seq_t)x;
        values[count++] = (seq_t)1;
    }

    /* find the mode: linear scan with counting */
    seq_t best = values[0];
    size_t bestcnt = 0;

    for (size_t i = 0; i < count; ++i) {
        seq_t v = values[i];
        size_t cnt = 0;
        for (size_t j = 0; j < count; ++j)
            if (values[j] == v) cnt++;

        if (cnt > bestcnt || (cnt == bestcnt && v < best)) {
            bestcnt = cnt;
            best = v;
        }
    }
    return best;
}

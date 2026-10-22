/*
 * A381576  a(n) is the second element of the sorted multiset of bases and
 *          exponents (including exponents = 1) in the prime factorization of
 *          n.
 * https://oeis.org/A381576
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the second smallest element of the multiset of bases and
 *            exponents (including exponent 1) in the prime factorization of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* defined only for n ≥ 2 */

    uint64_t tmp = (uint64_t)n;
    /* collect all bases and exponents in a small fixed array */
    seq_t values[64];
    size_t cnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0) continue;

        /* base p */
        values[cnt++] = (seq_t)p;

        /* exponent of p */
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        values[cnt++] = (seq_t)exp;
    }

    if (tmp > 1) {               /* remaining prime factor */
        values[cnt++] = (seq_t)tmp; /* base */
        values[cnt++] = (seq_t)1;   /* exponent */
    }

    /* find the minimum and its count */
    seq_t min1 = values[0];
    for (size_t i = 1; i < cnt; ++i) {
        if (values[i] < min1) min1 = values[i];
    }
    size_t count_min1 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (values[i] == min1) ++count_min1;
    }

    if (count_min1 >= 2) return min1; /* duplicated smallest value */

    /* find the next distinct value */
    seq_t min2 = (seq_t)-1;
    for (size_t i = 0; i < cnt; ++i) {
        if (values[i] == min1) continue;
        if (min2 == (seq_t)-1 || values[i] < min2) min2 = values[i];
    }

    return min2;
}

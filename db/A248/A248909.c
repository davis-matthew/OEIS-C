/*
 * A248909  Completely multiplicative with a(p) = p if p = 6k+1 and a(p) = 1
 *          otherwise.
 * https://oeis.org/A248909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A248909:  Completely multiplicative with
 * a(p) = p if p ≡ 1 (mod 6), a(p) = 1 otherwise.
 * For general n, a(n) = product of pᵉ for each prime power pᵉ | n
 * where p ≡ 1 (mod 6).  Equivalently, in the prime factorisation of n,
 * replace every prime not of the form 6k+1 by 1.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    /* factor 2 */
    unsigned int cnt = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        cnt++;
    }
    /* 2 ≡ 2 (mod 6), so ignore */

    /* odd factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            if (p % 6 == 1) {
                for (unsigned int i = 0; i < e; ++i)
                    result *= (seq_t)p;
            }
        }
    }

    /* remainder is a prime > sqrt(original n) */
    if (tmp > 1 && tmp % 6 == 1)
        result *= (seq_t)tmp;

    return result;
}

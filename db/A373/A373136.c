/*
 * A373136  a(n) = 1 if n is of the form 2^i * 3^j * k, i+j even, gcd(k,6)=1,
 *          otherwise 0.
 * https://oeis.org/A373136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the sum of the exponents of 2 and 3 in n is even,
 *            otherwise 0. More formally, if n = 2^i * 3^j * k where
 *            gcd(k,6)=1, then a(n) = [ (i + j) mod 2 == 0 ]
 * Range:     u8 n <= 8876, u16 n <= 8876, u32 n <= 8876, u64 n <= 8876,
 *            u128 n <= 8876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Local copy of n for factorisation */
    uint64_t temp = n;
    uint64_t i = 0, j = 0;

    /* Count powers of 2 */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        i++;
    }

    /* Count powers of 3 */
    while (temp % 3ULL == 0) {
        temp /= 3ULL;
        j++;
    }

    /* Remaining factor k is coprime to 6 by construction.
       Return 1 if i+j is even, else 0. */
    return ((i + j) & 1ULL) ? (seq_t)0 : (seq_t)1;
}

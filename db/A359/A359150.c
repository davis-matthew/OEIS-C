/*
 * A359150  a(n) = 1 if n is a number of the form 4u+1 with an odd number of
 *          prime factors (counted with multiplicity), otherwise 0.
 * https://oeis.org/A359150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 1 (mod 4) and the total number of prime *
 *            factors of n (counted with multiplicity) is odd; otherwise 0
 * Range:     u8 n <= 9652, u16 n <= 9652, u32 n <= 9652, u64 n <= 9652,
 *            u128 n <= 9652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Only numbers of the form 4u+1 can be 1 */
    if ((n & 3) != 1) {
        return 0;
    }

    /* Count prime factors (with multiplicity).  n fits in uint64_t. */
    uint64_t m = (uint64_t)n;
    unsigned int count = 0;

    /* Handle factor 2 separately to avoid an if in the loop */
    while ((m & 1ULL) == 0ULL) {
        ++count;
        m >>= 1;
    }

    /* Odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            ++count;
            m /= p;
        }
    }

    if (m > 1) {
        ++count;           /* remaining prime > sqrt(original n) */
    }

    return (count & 1U) ? 1 : 0;
}

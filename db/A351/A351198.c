/*
 * A351198  Sum of the 10th powers of the primes dividing n.
 * https://oeis.org/A351198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^10
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 88, u128 n <= 7150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* safety for out‑of‑range */

    uint64_t temp = (uint64_t)n;
    seq_t sum = 0;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        seq_t p10 = 1;
        seq_t base = (seq_t)2ULL;
        for (int i = 0; i < 10; ++i)
            p10 *= base;
        sum += p10;
        while ((temp & 1ULL) == 0)
            temp >>= 1;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t p10 = 1;
            seq_t base = (seq_t)p;
            for (int i = 0; i < 10; ++i)
                p10 *= base;
            sum += p10;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* remaining prime factor */
    if (temp > 1) {
        seq_t p10 = 1;
        seq_t base = (seq_t)temp;
        for (int i = 0; i < 10; ++i)
            p10 *= base;
        sum += p10;
    }

    return sum;
}

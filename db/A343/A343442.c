/*
 * A343442  If n = Product (p_j^k_j) then a(n) = Product (p_j + 2), with a(1)
 *          = 1.
 * https://oeis.org/A343442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p|n} (p + 2) with a(1) = 1
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)                    /* a(1) = 1, and any n ≤ 0 is out of range */
        return (seq_t)1;

    seq_t res = (seq_t)1;
    uint64_t m = (uint64_t)n;      /* work on a 64‑bit copy */

    /* Handle the prime factor 2 separately */
    if ((m & 1ULL) == 0) {
        res *= (seq_t)(2 + 2);
        while ((m & 1ULL) == 0)
            m >>= 1;
    }

    /* Trial division by odd candidates up to sqrt(m) */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            res *= (seq_t)(p + 2);
            while (m % p == 0)
                m /= p;
        }
    }

    /* If a prime factor > sqrt(n) remains, it is m itself */
    if (m > 1)
        res *= (seq_t)(m + 2);

    return res;
}

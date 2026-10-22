/*
 * A354969  For prime partition (k_1, k_2, ..., k_i) of the number x, let the
 *          conjugate partition be (m_1, m_2, ... , m_j). If n = k_1 * k_2 *
 *          ... * k_i, then a(n) = m1 * m2 * ... * m_j.
 * https://oeis.org/A354969
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over t = 1..max_prime(n) of * the number of prime
 *            factors of n that are ≥ t * (i.e., product of the parts of the
 *            conjugate partition of the * multiset of prime factors of n)
 * Range:     u8 n <= 97, u16 n <= 288, u32 n <= 1368, u64 n <= 4488,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* offset starts at 2 */

    /* at most 20 prime factors for n ≤ 10000, but we allow some margin */
    seq_t parts[32];
    size_t k = 0;

    idx_t temp = n;
    /* trial division; works for all n < 2^63 as well */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        while (temp % p == 0)
        {
            parts[k++] = (seq_t)p;
            temp /= p;
        }
    }
    if (temp > 1)
        parts[k++] = (seq_t)temp;  /* remaining prime factor */

    /* find the largest part (largest prime factor) */
    seq_t maxp = 0;
    for (size_t i = 0; i < k; ++i)
        if (parts[i] > maxp)
            maxp = parts[i];

    /* compute the product of the conjugate partition parts */
    seq_t prod = 1;
    for (seq_t t = 1; t <= maxp; ++t)
    {
        idx_t cnt = 0;
        for (size_t i = 0; i < k; ++i)
            if (parts[i] >= t)
                ++cnt;
        prod = prod * (seq_t)cnt;
    }

    return prod;
}

/*
 * A273282  Largest prime not exceeding the geometric mean of all prime
 *          divisors of n counted with multiplicity.
 * https://oeis.org/A273282
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime ≤ GM(n) where GM(n) is the geometric * mean
 *            of all prime divisors of n counted with multiplicity. * For n≥2,
 *            a(n) = n if n is prime, otherwise a(n) is the largest * prime
 *            not exceeding n^(1/ω(n)), where ω(n) is the total number of *
 *            prime factors of n counted with multiplicity
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors_u64, is_prime, power_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(n) for idx_t */
#include "../utils/count_prime_factors_u64.h"
#include "../utils/power_le.h"

static idx_t kth_root_floor(idx_t n, idx_t k)
{
    /* Return floor(n^(1/k)) for k>=1 and 0< n <= UINT64_MAX. */
    if (k == 0)
        return 0; /* Undefined, not used. */
    if (k == 1)
        return n;

    idx_t low = 1, high = n, ans = 1;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (power_le(mid, k, n))
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;       /* Undefined for n < 2. */

    /* If n is prime, the answer is n itself. */
    if (is_prime(n))
        return (seq_t)n;

    /* Compute ω(n) – total number of prime factors with multiplicity. */
    idx_t omega = count_prime_factors_u64((uint64_t)n);

    /* Compute floor of the geometric mean: floor(n^(1/ω(n))). */
    idx_t root = kth_root_floor(n, omega);

    /* Find the largest prime ≤ root. */
    for (idx_t cand = root; cand >= 2; --cand)
    {
        if (is_prime(cand))
            return (seq_t)cand;
        /* Avoid unsigned underflow: stop when cand reaches 2. */
        if (cand == 2)
            break;
    }

    /* Should never reach here for n ≥ 2. */
    return (seq_t)1;
}

/*
 * A242425  Number of squares k^2 < prime(n) with k^2*p == 1 (mod prime(n))
 *          for some prime p < prime(n).
 * https://oeis.org/A242425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of squares k^2 < prime(n) for which the inverse of
 *            k^2 modulo prime(n) is a prime less than prime(n)
 * Range:     u8 n <= 54, u16 n <= 6800, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, get_prime, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, unused for n<1 */

    /* n-th prime */
    uint32_t p_u = get_prime(n);
    seq_t p_seq = (seq_t)p_u;      /* prime(n) as seq_t */

    /* largest k with k^2 < p */
    idx_t max_k = _floor_isqrt_u64((idx_t)p_u - 1UL);
    seq_t count = 0;

    for (idx_t k = 1; k <= max_k; ++k)
    {
        seq_t s   = (seq_t)k * (seq_t)k;                    /* k^2 */
        seq_t inv = mod_pow(s, (uint64_t)(p_u - 2U), p_seq);  /* inverse of s mod p */

        if (inv < p_seq && is_prime((idx_t)inv))
            ++count;
    }
    return count;
}

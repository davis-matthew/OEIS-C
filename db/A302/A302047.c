/*
 * A302047  a(n) = 1 if n = prime(k)*prime(2+k) for some k, otherwise 0.
 * https://oeis.org/A302047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8073, u16 n <= 8073, u32 n <= 8073, u64 n <= 8073,
 *            u128 n <= 8073, big any n
 * Helpers:   build_primes_upto, compute_pi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by checking if n equals p_k * p_{k+2} (product of a prime
   and the prime two places after it in the prime list).  Returns 1 if so,
   otherwise 0. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/compute_pi.h"
#include "../utils/build_primes_upto.h"

/* Global cache of primes */
static uint64_t *primes = NULL;
static size_t   prime_cnt = 0;
static uint64_t  prime_limit = 0;

/* Build a fresh list of primes up to 'limit'.
   Old list is freed. */
static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;

    uint64_t *new_primes = NULL;
    size_t   new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    free(primes);
    primes = new_primes;
    prime_cnt = new_cnt;
    prime_limit = limit;
}

/* Integer square‑root: floor(sqrt(n)).  Works for 0 <= n < 2^64. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t low = 0, high = (uint64_t)1 << 32;  /* > sqrt(UINT64_MAX) */
    while (low <= high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = 1; continue; }   /* avoid division by 0 */
        uint64_t div = n / mid;
        if (mid <= div) low = mid + 1;
        else            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
/* Return 1 if n is a product of a prime and the prime two places after
   it in the list of primes; otherwise return 0. */
{
    if (n < 2) return 0;          /* 1 cannot be such a product */

    uint64_t N = (uint64_t)n;
    uint64_t limit = isqrt_u64(N) + 1;
    ensure_primes(limit);

    uint64_t p = 0, q = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t cur = primes[i];
        if (cur * cur > N) break;
        if (N % cur == 0)
        {
            p = cur;
            q = N / cur;
            break;
        }
    }

    if (p == 0) return 0;                /* N is prime (or 1), cannot be product */

    if (!is_prime((idx_t)q)) return 0;    /* q must also be prime */

    /* Compute prime indices of p and q. */
    seq_t pi_p = compute_pi((idx_t)p);
    seq_t pi_q = compute_pi((idx_t)q);

    /* Compare indices; diff = |pi_p - pi_q|. */
    seq_t diff = (pi_p > pi_q) ? (pi_p - pi_q) : (pi_q - pi_p);

    return (diff == 2) ? 1 : 0;
}

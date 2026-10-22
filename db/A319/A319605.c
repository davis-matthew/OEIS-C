/*
 * A319605  a(1) = 1, and for n > 1, a(n) is the least prime power of the form
 *          p^k >= n where p is a prime factor of n.
 * https://oeis.org/A319605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime‑power ≥ n such that the base prime divides n
 *            (a(1) = 1). The algorithm factors n by trial division using a
 *            cache of primes up to ⌊√n⌋. For each prime factor p it builds
 *            the smallest power p^k ≥ n and keeps the minimum among those
 * Range:     u8 n <= 33, u16 n <= 4960, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

static idx_t *prime_list = NULL;      /* cached prime list as idx_t */
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Build (or extend) prime_list up to at least 'limit'.  The function
   build_primes_upto returns a uint64_t* which is compatible with idx_t. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_list) free(prime_list);
    uint64_t *tmp;
    size_t cnt;
    build_primes_upto(limit, &tmp, &cnt);
    prime_list = (idx_t *)tmp;
    prime_count = cnt;
    prime_limit = limit;
}

/* Computes A319605(n) for n ≥ 1.  The function is independent of call
   order and accepts arbitrary‑size accounter types. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    idx_t temp = n;
    seq_t best = 0;           /* 0 used as "unset" sentinel */
    idx_t sqrt_n = _floor_isqrt_u64(n);

    /* Ensure we can test divisibility by all primes ≤ √n */
    if (sqrt_n < 2) sqrt_n = 2;
    ensure_primes_upto(sqrt_n);

    /* Loop over all primes up to √n that divide temp */
    for (size_t i = 0; i < prime_count && prime_list[i] <= sqrt_n && temp > 1; ++i) {
        idx_t p = prime_list[i];
        if (temp % p == 0) {
            /* Compute the smallest power of p that is ≥ n */
            seq_t power = (seq_t)p;
            while (power < (seq_t)n) power *= (seq_t)p;
            if (best == 0 || power < best) best = power;

            /* Remove all copies of p from temp */
            while (temp % p == 0) temp /= p;
        }
    }

    /* If a prime factor larger than √n remains, it must be at most one
       prime (the remaining temp itself). */
    if (temp > 1) {
        idx_t p = temp;
        seq_t power = (seq_t)p;
        while (power < (seq_t)n) power *= (seq_t)p;
        if (best == 0 || power < best) best = power;
    }

    /* best should now hold the answer (it is guaranteed to be set). */
    return best;
}

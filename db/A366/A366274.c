/*
 * A366274  a(n) is the least k such that prime(n+1+k) >= prime(n)+prime(n+1).
 * https://oeis.org/A366274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that prime(n+1+k) >= prime(n)+prime(n+1)
 * Range:     u8 n <= 30, u16 n <= 4362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a cached sieve of primes generated on demand with
 * the helper build_primes_upto. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;    /* array of primes, 0‑based */
static size_t prime_cnt = 0;       /* number of primes stored in primes */
static idx_t   sieve_limit = 0;    /* current upper bound used for the sieve */

/* Ensure that primes contains at least (need_index+1) primes
   (i.e. indices 0 … need_index are valid). */
static void ensure_primes_up_to_index(idx_t need_index)
{
    idx_t required_cnt = need_index + 1;
    if (prime_cnt >= required_cnt)
        return;                       /* already enough */

    if (sieve_limit == 0)             /* first call, start small */
        sieve_limit = 100;

    while (prime_cnt < required_cnt) {
        free(primes);                  /* release previous allocation */
        build_primes_upto(sieve_limit, &primes, &prime_cnt);
        if (prime_cnt >= required_cnt)
            break;
        sieve_limit *= 2;              /* increase bound and try again */
    }
}

/* Ensure that the sieve contains a prime >= target.  This may enlarge
 * the sieve if necessary. */
static void ensure_prime_cover(seq_t target)
{
    while (prime_cnt > 0 && primes[prime_cnt - 1] < target) {
        free(primes);
        sieve_limit *= 2;
        build_primes_upto(sieve_limit, &primes, &prime_cnt);
    }
}

/* Find the first index i >= start such that primes[i] >= value.
   Assumes primes[end] exists and is >= value. */
static idx_t lower_bound_idx(idx_t start, seq_t value)
{
    idx_t lo = start;
    idx_t hi = prime_cnt;               /* exclusive upper bound */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (primes[mid] >= value)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first prime 2, so it is defined for n >= 1. */
    if (n == 0)
        return 0;                       /* undefined for n = 0, return 0 */

    /* We need primes up to at least index n+1 to obtain prime(n) and prime(n+1). */
    ensure_primes_up_to_index(n + 1);

    seq_t p_n   = (seq_t)primes[n - 1];  /* prime(n)   */
    seq_t p_np1 = (seq_t)primes[n];      /* prime(n+1) */
    seq_t target = p_n + p_np1;          /* prime(n)+prime(n+1) */

    /* Ensure the sieve contains a prime >= target. */
    ensure_prime_cover(target);

    /* Binary search for the smallest i >= n such that primes[i] >= target. */
    idx_t idx = lower_bound_idx(n, target);

    /* k = idx - n;  idx and n are 1‑based indices offset by zero‑based array. */
    seq_t k = (seq_t)(idx - n);
    return k;
}

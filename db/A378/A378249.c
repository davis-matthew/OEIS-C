/*
 * A378249  Least perfect power > prime(n).
 * https://oeis.org/A378249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 30, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A378249
   Least perfect power > prime(n), offset 1.
   a(n) = min{ k^e : e≥2, k≥2, k^e > prime(n) } */

/* 4, 4, 8, 8, 16, 16, 25, 25, 25, 32, 32, 49, 49, 49, 49, 64, 64, 64, 81, 81,
    81, 81, 100, 100, … */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility functions provided by the harness */
#include "../utils/big_pow.h"         /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/build_primes_upto.h"/* build_primes_upto(idx_t limit, uint64_t **p, size_t *cnt) */

/* Return the floor of the e‑th root of n :  the largest integer r with r^e ≤ n.
   Uses binary search with big_pow. */
static seq_t floor_root(seq_t n, idx_t e)
{
    seq_t low  = 1;          /* root is always ≥1 for n≥1 */
    seq_t high = n;          /* n itself is an upper bound */
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t pw  = big_pow(mid, (seq_t)e);
        if (pw <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* safety against underflow */
            high = mid - 1;
        }
    }
    return high;             /* largest mid with mid^e ≤ n */
}

/* Sequence function.  The nth prime is retrieved from a cached prime list.
   For each exponent e from 2 up to ⌊log₂ p⌋+1 we compute the smallest
   perfect‑power exceeding p, and keep the minimum.  The list of primes
   is rebuilt if the requested index exceeds the current limit. */
seq_t a(idx_t n)
{
    /* Prime cache – initialised on first call. */
    static bool          initialized = false;
    static uint64_t     *primes      = NULL;
    static size_t        prime_cnt   = 0;
    static idx_t         prime_limit = 0;

    /* Ensure we have at least n primes.  Start with a coarse limit and double
       when necessary. */
    if (!initialized || n > prime_cnt) {
        idx_t  new_limit = prime_limit ? prime_limit * 2 : 200000;
        while (true) {
            uint64_t *tmp, *old = primes;
            size_t   old_cnt = prime_cnt;

            build_primes_upto(new_limit, &tmp, &prime_cnt);
            if (prime_cnt >= n) {
                primes      = tmp;
                prime_limit = new_limit;
                free(old);
                break;
            }
            /* Not enough primes – add more */
            free(tmp);
            new_limit <<= 1;
        }
        initialized = true;
    }

    /* Get the nth prime (1‑based). */
    seq_t p = (seq_t)primes[n - 1];

    /* Determine maximum exponent to test:
       e_max = ⌊log₂ p⌋ + 1.  For 2^e > p we only need exponents up to e_max.  */
    idx_t e = 1;
    seq_t pow2 = (seq_t)2;
    while (pow2 <= p) {
        pow2 *= 2;
        ++e;
    }
    idx_t e_max = e;  /* e is now first exponent with 2^e > p, so we test up to e */

    /* Find the minimal perfect power greater than p. */
    seq_t best  = 0;
    bool  first = true;
    for (idx_t exp = 2; exp <= e_max; ++exp) {
        /* Largest integer r with r^exp ≤ p */
        seq_t root = floor_root(p, exp);
        /* Candidate base: one more than that root */
        seq_t base = root + 1;

        /* Compute base^exp */
        seq_t cand = big_pow(base, (seq_t)exp);

        /* Keep the smallest candidate that is > p */
        if (cand > p && (first || cand < best)) {
            best  = cand;
            first = false;
        }
    }
    return best;
}

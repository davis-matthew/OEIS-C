/*
 * A242398  Partial sums of the number of primes separating successive pairs
 *          of twin primes.
 * https://oeis.org/A242398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 8597, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A242398.
   Formula: a(n) = sum_{k=2}^{n+1} #{primes p | p is not in a twin pair,
                        p > first twin pair (3,5)   and   p < k-th twin pair}.
   Implementation: sieve primes up to a fixed limit sufficient for n ≈ 10 000,
   find all twin‑prime pairs, precompute the cumulative counts, and cache
   them for repeated calls. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static seq_t *a_values = NULL;   /* cumulative sums a(1) … a(a_len)   */
static size_t a_len = 0;         /* number of computed terms          */
static bool initialized = false;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safety */

    /* --- lazy precomputation ------------------------------------- */
    if (!initialized)
    {
        /* choose a limit that covers at least 10 000 twin pairs */
        const idx_t LIMIT = 4000000ULL;

        /* 1. generate prime list */
        uint64_t *primes = NULL;
        size_t primes_cnt = 0;
        build_primes_upto(LIMIT, &primes, &primes_cnt);

        /* 2. locate twin‑prime pairs (index of first prime of each pair) */
        size_t *twin_idx = (size_t*)malloc(primes_cnt * sizeof(size_t));
        size_t twin_cnt = 0;
        for (size_t i = 0; i + 1 < primes_cnt; ++i)
            if (primes[i + 1] - primes[i] == 2)
                twin_idx[twin_cnt++] = i;

        /* 3. mark primes that belong to any twin pair */
        bool *twin_prime_flag = (bool*)calloc(primes_cnt, sizeof(bool));
        for (size_t i = 0; i < twin_cnt; ++i) {
            twin_prime_flag[twin_idx[i]]     = true;
            twin_prime_flag[twin_idx[i] + 1] = true;
        }

        /* 4. prefix sum of twin‑prime flags for fast range queries */
        size_t *twin_pref = (size_t*)malloc((primes_cnt + 1) * sizeof(size_t));
        twin_pref[0] = 0;
        for (size_t i = 0; i < primes_cnt; ++i)
            twin_pref[i + 1] = twin_pref[i] + (twin_prime_flag[i] ? 1 : 0);

        /* 5. compute cumulative sums of non‑twin primes between successive
           twin pairs */
        a_len = (twin_cnt > 1 ? twin_cnt - 1 : 0);      /* a(1)…a(len) */
        if (a_len > 0)
            a_values = (seq_t*)malloc(a_len * sizeof(seq_t));

        seq_t running = 0;
        for (size_t i = 0; i + 1 < twin_cnt; ++i) {
            size_t i1 = twin_idx[i];
            size_t i2 = twin_idx[i + 1];

            /* total primes strictly between the two pairs:
               indices i1+1 … i2-1  (count = i2 - i1 - 1) */
            size_t total_between = (i2 > i1 + 1) ? (i2 - i1 - 1) : 0;

            /* subtract primes that belong to any twin pair inside the range */
            size_t twin_between = twin_pref[i2] - twin_pref[i1 + 1];
            seq_t add = (seq_t)(total_between - twin_between);

            running += add;
            a_values[i] = running;
        }

        /* free temporary structures */
        free(primes);
        free(twin_idx);
        free(twin_prime_flag);
        free(twin_pref);

        initialized = true;
    }

    /* --- answer query ---------------------------------------------- */
    if (a_len == 0) return (seq_t)0;          /* no twin pairs found */

    /* clamp to the precomputed range */
    if (n > a_len) n = a_len;

    return a_values[n - 1];   /* a(1) is stored at index 0 */
}

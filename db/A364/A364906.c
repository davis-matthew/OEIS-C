/*
 * A364906  Number of ways to write A056239(n) as a nonnegative linear
 *          combination of the multiset of prime indices of n.
 * https://oeis.org/A364906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write A056239(n) as a nonnegative *
 *            linear combination of the multiset of prime indices of n. Here *
 *            A056239(n) = Σ_i e_i * i where n = Π_i p_i^{e_i} and i is the *
 *            1‑based index of prime p_i
 * Range:     u8 n <= 63, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

static idx_t *primes = NULL;           /* list of primes ≤ current limit */
static size_t prime_count = 0;
static idx_t prime_limit = 0;

static uint32_t *prime_index_arr = NULL; /* prime_index_arr[p] = index of p (1‑based), 0 if not prime */
static size_t prime_index_arr_size = 0;

/* Ensure that primes list and index array are computed up to limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    idx_t *new_primes = NULL;
    size_t new_cnt = 0;
    /* build_primes allocates and returns primes ≤ limit */
    build_primes(limit, &new_primes, &new_cnt);

    /* Free old list (if any). */
    free(primes);
    primes = new_primes;
    prime_count = new_cnt;
    prime_limit = limit;

    /* Resize index array. */
    size_t new_size = (size_t)limit + 1;
    uint32_t *new_arr = (uint32_t *)realloc(prime_index_arr,
                                            new_size * sizeof(uint32_t));
    if (!new_arr) /* allocation failure – we keep old array as a fallback */
        return;
    prime_index_arr = new_arr;
    prime_index_arr_size = new_size;
    /* Zero initialize new positions. */
    for (size_t i = 0; i < new_size; ++i)
        prime_index_arr[i] = 0;

    /* Fill indices for each prime. */
    for (size_t i = 0; i < prime_count; ++i)
        prime_index_arr[(size_t)primes[i]] = (uint32_t)(i + 1); /* 1‑based */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset is 1; values below are irrelevant */

    if (n == 1)
        return 1;           /* A056239(1)=0 → only empty combination */

    /* Prepare primes up to n for factorisation and index lookup. */
    ensure_primes_upto(n);

    /* Factorise n, storing indices and exponents. */
    uint64_t nn = n;
    size_t pf_cnt = 0;
    uint32_t idxs[16];
    size_t exps[16];

    /* Trial division using the primes list. */
    for (size_t pi = 0; pi < prime_count && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= nn; ++pi) {
        uint64_t p = primes[pi];
        if (nn % p != 0)
            continue;
        size_t exp = 0;
        while (nn % p == 0) {
            nn /= p;
            ++exp;
        }
        idxs[pf_cnt] = prime_index_arr[(size_t)p];
        exps[pf_cnt] = exp;
        ++pf_cnt;
    }
    /* Remaining prime factor, if any. */
    if (nn > 1) {
        uint64_t p = nn;
        idxs[pf_cnt] = prime_index_arr[(size_t)p];
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Compute target sum S = Σ e_i * index_i. */
    size_t S = 0;
    for (size_t i = 0; i < pf_cnt; ++i)
        S += (size_t)exps[i] * (size_t)idxs[i];

    /* DP array for counting solutions: dp[t] = number of ways to reach sum t. */
    seq_t *dp = (seq_t *)calloc(S + 1, sizeof(seq_t));
    if (!dp)   /* memory error – the caller may handle it */
        return 0;

    dp[0] = 1; /* empty combination yields sum 0 */

    /* For each prime factor copy, update DP. */
    for (size_t i = 0; i < pf_cnt; ++i) {
        size_t weight = (size_t)idxs[i];
        size_t copies = exps[i];
        for (size_t c = 0; c < copies; ++c) {
            for (size_t t = weight; t <= S; ++t) {
                dp[t] += dp[t - weight];
            }
        }
    }

    seq_t result = dp[S];
    free(dp);
    return result;
}

/*
 * A398694  Smallest 2-dense number (A174973) with n odd divisors.
 * https://oeis.org/A398694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, new
 * Formula:   a(1) = 1; for n > 1, a(n) = 2 * m, where m is the smallest odd
 *            integer whose divisor function satisfies d(m) = n. The function
 *            implements this definition by computing, for each n, the minimal
 *            odd number with exactly n divisors and then multiplying it by 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, seq_cmp, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Utility helpers */
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_cmp.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/* Odd‑prime list (excluding 2)                                          */
/* ------------------------------------------------------------------ */
static uint64_t *odd_primes = NULL;
static size_t odd_prime_cnt = 0;

static void init_odd_primes(void)
{
    if (odd_primes != NULL)
        return;

    /* 200 000 is large enough for the required range of n. */
    uint64_t *all_primes = NULL;
    size_t all_cnt = 0;
    build_primes_upto(200000, &all_primes, &all_cnt);

    odd_prime_cnt = (all_cnt > 1) ? all_cnt - 1 : 0;
    odd_primes = (uint64_t *)malloc(odd_prime_cnt * sizeof(uint64_t));

    for (size_t i = 1; i < all_cnt; ++i)             /* skip prime 2 */
        odd_primes[i - 1] = all_primes[i];

    free(all_primes);
}

/* ------------------------------------------------------------------ */
/* Depth‑first search for the minimal odd number with a given divisor
   count.  The search explores exponent vectors (e1, e2, …) such that
   (e1+1)(e2+1)… = target.  The exponents are required to be
   non‑increasing so that each number is considered only once.       */
/* ------------------------------------------------------------------ */
static void dfs(size_t prime_index,
                idx_t rem_n,
                idx_t max_exp,
                seq_t cur_prod,
                seq_t *best,
                bool *found)
{
    /* All divisor factors obtained: cur_prod is a candidate. */
    if (rem_n == 1) {
        if (!*found || seq_cmp(&cur_prod, best) < 0) {
            *best = cur_prod;
            *found = true;
        }
        return;
    }

    if (prime_index >= odd_prime_cnt)
        return;

    /* Prune: if we already have a candidate and cur_prod is no better
       than it, no need to continue. */
    if (*found && seq_cmp(&cur_prod, best) >= 0)
        return;

    uint64_t p = odd_primes[prime_index];
    for (idx_t e = 1; e <= max_exp; ++e) {
        if (rem_n % (e + 1) != 0)
            continue;

        seq_t prime_pow = big_pow((seq_t)p, (seq_t)e);
        seq_t next_prod = seq_mul(cur_prod, prime_pow);
        dfs(prime_index + 1,
            rem_n / (e + 1),
            e,
            next_prod,
            best,
            found);
        /* No explicit free needed: prime_pow and next_prod are value types. */
    }
}

/* Minimal odd integer whose divisor count equals n. */
static seq_t minimal_odd_with_divcount(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* only odd divisor is 1 */

    init_odd_primes();

    seq_t best = (seq_t)0;
    bool found = false;
    dfs(0, n, 64, (seq_t)1, &best, &found);   /* 64 is a safe upper bound for exponents */

    return best;   /* guaranteed found for n >= 1 */
}

/* ------------------------------------------------------------------ */
/* Public function: a(n)                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Cache results for repeated calls. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    if (cache[n] != (seq_t)0)          /* already computed */
        return cache[n];

    seq_t odd_part = minimal_odd_with_divcount(n);
    seq_t result = seq_mul((seq_t)2, odd_part);   /* ensure 2‑dense property */

    cache[n] = result;
    return result;
}

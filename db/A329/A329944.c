/*
 * A329944  Number of permutations of [n] whose cycle lengths avoid primes.
 * https://oeis.org/A329944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) + Σ_{k composite, 4≤k≤n} (n‑1)!/(n‑k)! · a(n‑k)
 *            with a(0)=1. Sequence A329944: number of permutations of [n]
 *            whose cycle lengths avoid primes
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *perm_cache = NULL;      /* a[n] values */
static idx_t   cache_size = 0;        /* number of cached results */

static bool *prime_flag = NULL;        /* prime sieve flags up to prime_limit */
static idx_t   prime_limit = 0;

/* Ensure prime_flag contains prime information up to n           */
static void ensure_prime_sieve(idx_t n)
{
    if (prime_limit >= n)
        return;

    /* Grow array and init new entries as true */
    bool *new_flag = (bool *)realloc(prime_flag, (n + 1) * sizeof(bool));
    if (!new_flag)                   /* allocation failure – let caller fail later */
        return;
    prime_flag = new_flag;
    for (idx_t i = prime_limit + 1; i <= n; ++i)
        prime_flag[i] = true;
    if (n >= 0) prime_flag[0] = false;
    if (n >= 1) prime_flag[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                prime_flag[j] = false;
        }
    }
    prime_limit = n;
}

/* Extend perm_cache to include terms up to n (inclusive) */
static void extend_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Make sure prime information is available */
    ensure_prime_sieve(n);

    /* Grow cache array */
    seq_t *new_cache = (seq_t *)realloc(perm_cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)                   /* allocation failure – let caller fail later */
        return;
    perm_cache = new_cache;

    /* Initialise first element if needed */
    if (cache_size == 0) {
        perm_cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Compute new terms */
    for (idx_t cur = cache_size; cur <= n; ++cur) {
        if (cur == 0)                /* already set above */
            continue;

        seq_t sum = (seq_t)0;

        /* k = 1 term: fixed point */
        sum += perm_cache[cur - 1];

        /* k ≥ 2 composite terms */
        seq_t ratio = (seq_t)1;          /* product of numbers from cur‑k+1 to cur‑1 */
        idx_t mult = cur - 1;             /* next number to multiply for k = 2 */

        for (idx_t k = 2; k <= cur; ++k) {
            ratio *= (seq_t)mult;            /* include factor cur‑k+1 */
            if (!prime_flag[k])              /* composite k (k≥4 automatically) */
                sum += ratio * perm_cache[cur - k];
            mult--;                            /* prepare for next k */
        }
        perm_cache[cur] = sum;
    }
    cache_size = n + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    extend_cache(n);
    return perm_cache[n];
}

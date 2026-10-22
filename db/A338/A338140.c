/*
 * A338140  a(n) is the smallest number with n refactorable divisors.
 * https://oeis.org/A338140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest number with exactly n refactorable divisors
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include necessary utilities */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Forward declaration */
static int count_refactorable(uint64_t k);

/* Main function */
seq_t a(idx_t n)
{
    /* Static cache: minimal k for each count value */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static uint64_t next_k = 1;          /* next number to test */

    /* Ensure cache is large enough to hold index n */
    if (cache_size <= n) {
        size_t new_size = cache_size ? cache_size << 1 : 128;
        if (new_size <= n) new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    /* Expand cache if necessary and compute until a(n) is known */
    while (cache[n] == 0) {
        int cnt = count_refactorable(next_k);
        if ((idx_t)cnt < cache_size && cache[cnt] == 0) {
            cache[cnt] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n];
}

/* ------------------------------------------------------------------ */
/* Helper routine: count refactorable divisors of k */
/* ------------------------------------------------------------------ */
/* Recursive traversal over all divisors of k */
static void dfs_divisors(int idx, int pcnt,
                         uint64_t val, uint64_t tau,
                         const uint64_t *pr, const uint64_t *ex,
                         int *cnt)
{
    if (idx == pcnt) {
        if (val % tau == 0) ++(*cnt);
        return;
    }
    uint64_t p = pr[idx];
    int e = (int)ex[idx];
    uint64_t d = 1;
    for (int f = 0; f <= e; ++f) {
        dfs_divisors(idx + 1, pcnt,
                     val * d, tau * (f + 1),
                     pr, ex, cnt);
        d *= p;
    }
}

/* Count refactorable divisors for a single integer k */
static int count_refactorable(uint64_t k)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(k, primes, exps, &pcnt);

    int cnt = 0;
    dfs_divisors(0, (int)pcnt, 1, 1, primes, exps, &cnt);
    return cnt;
}

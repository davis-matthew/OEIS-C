/*
 * A361493  Expansion of e.g.f. exp(exp(x) - 1 + x^3).
 * https://oeis.org/A361493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A361493:  Expansion of e.g.f. exp(exp(x) - 1 + x^3).
    Recurrence:
        a(0) = a(1) = 1, a(2) = 2
        a(n) = 6 * C(n-1,2) * a(n-3) + Σ_{k=1}^{n} C(n-1,k-1) * a(n-k)
    The code memoises computed values and evaluates the
    recurrence exactly using seq_t arithmetic. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* cache for previously computed terms */
static seq_t *cache      = NULL; /* array of terms */
static idx_t  cache_len  = 0;    /* length of allocated cache (number of slots) */
static idx_t  computed   = 0;    /* highest index that has a value in cache */
static bool   initialized = false;

/* enlarge the cache if necessary so that indices 0..need are all valid */
static void ensure_capacity(idx_t need)
{
    if (!cache) {
        cache_len = 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    }
    if (need >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= need) new_len <<= 1;   /* grow fast enough */
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = tmp;
        cache_len = new_len;
    }
}

/* the sequence element requested by the caller */
seq_t a(idx_t n)
{
    /* we need space for indices up to max(n,2) to be able to set the base values */
    idx_t need = (n > 2) ? n : 2;
    ensure_capacity(need);

    /* set base values on first use */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        computed = 2;
        initialized = true;
    }

    /* compute any new terms required to reach n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* term: 6 * binomial(i-1,2) * a(i-3) */
        seq_t comb_2 = ((seq_t)(i - 1) * (seq_t)(i - 2)) / (seq_t)2; /* C(i-1,2) */
        seq_t term = ((comb_2 * (seq_t)6) * cache[i - 3]);

        /* sum over k: binomial(i-1,k-1) * a(i-k)   (k runs from 1 to i) */
        seq_t bin = (seq_t)1;   /* binomial(i-1,0) */
        seq_t sum = term;

        for (idx_t k = 1; k <= i; ++k) {
            sum += bin * cache[i - k];
            if (k < i) {
                /* update bin to C(i-1,k) from C(i-1,k-1) */
                bin = (bin * (seq_t)(i - k)) / (seq_t)k;
            }
        }

        cache[i] = sum;
    }

    computed = (computed > n) ? computed : n;
    return cache[n];
}

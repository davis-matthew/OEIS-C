/*
 * A254703  Number of length 5+4 0..n arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) Initial values: a(1)=136,
 *            a(2)=2310, a(3)=16276, a(4)=72271, a(5)=242076, a(6)=670372,
 *            a(7)=1618264, a(8)=3520845
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 25, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static const seq_t init_terms[9] = {
    0,
    (seq_t)136,
    (seq_t)2310,
    (seq_t)16276,
    (seq_t)72271,
    (seq_t)242076,
    (seq_t)670372,
    (seq_t)1618264,
    (seq_t)3520845
};

static seq_t *cache      = NULL;   /* memoized terms, 1‑indexed */
static size_t cache_size = 0;      /* allocated size */
static size_t computed_until = 0;  /* highest index computed */

/* Grow the cache so that it can store terms up to `need` (inclusive) */
static void ensure_capacity(size_t need)
{
    if (cache_size <= need) {
        size_t new_size = cache_size == 0 ? 16 : cache_size * 2;
        while (new_size <= need) new_size *= 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }
}

/* Initialise the first eight terms if not done yet */
static void init_initial_terms(void)
{
    if (computed_until == 0) {
        cache[0] = 0;  /* unused */
        for (size_t i = 1; i <= 8; ++i) cache[i] = init_terms[i];
        computed_until = 8;
    }
}

/* Compute all terms up to `upto` using the recurrence */
static void compute_to(size_t upto)
{
    if (computed_until >= upto) return;
    init_initial_terms();
    for (size_t i = computed_until + 1; i <= upto; ++i) {
        /* Positive part: 8*a(i-1) + 56*a(i-3) + 56*a(i-5) + 8*a(i-7) */
        seq_t pos = mul_small(cache[i - 1], 8);
        pos += mul_small(cache[i - 3], 56);
        pos += mul_small(cache[i - 5], 56);
        pos += mul_small(cache[i - 7], 8);

        /* Negative part: 28*a(i-2) + 70*a(i-4) + 28*a(i-6) + a(i-8) */
        seq_t neg = mul_small(cache[i - 2], 28);
        neg += mul_small(cache[i - 4], 70);
        neg += mul_small(cache[i - 6], 28);
        neg += cache[i - 8];

        cache[i] = pos - neg;  /* pos ≥ neg for all n > 8 */
    }
    computed_until = upto;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n==0, return 0 */
    size_t idx = (size_t)n;          /* n ≤ 10000 in practice */
    if (idx >= 8) {
        ensure_capacity(idx);
        compute_to(idx);
    } else {
        /* For n <= 8, init_terms is guaranteed to be available */
        if (cache == NULL) {
            ensure_capacity(8);
            init_initial_terms();
        }
    }
    return cache[idx];
}

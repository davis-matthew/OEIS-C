/*
 * A365604  Expansion of e.g.f. 1 / (1 - 5 * log(1 + x)).
 * https://oeis.org/A365604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} 5^k * k! * Stirling1(n,k) Recurrence (used
 *            in implementation): a(0)=1; a(n) = 5 * Σ_{k=1..n} (-1)^(k-1) *
 *            (k-1)! * C(n,k) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact = NULL;          /* factorials */
static idx_t fact_max = 0;
static size_t fact_cap = 0;

static seq_t *a_cache = NULL;       /* computed terms of the sequence */
static idx_t a_cache_max = 0;
static size_t a_cache_cap = 0;

/* Ensure factorial array holds values up to n */
static void need_factorial(idx_t n)
{
    if (n < fact_max) return;

    if (fact == NULL) {
        fact_cap = 1;
        fact = (seq_t*)malloc(sizeof(seq_t));
        fact[0] = 1;
        fact_max = 0;
    }
    if (n + 1 > fact_cap) {
        fact_cap = n + 1;
        fact = (seq_t*)realloc(fact, fact_cap * sizeof(seq_t));
    }
    for (idx_t i = fact_max + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    fact_max = n;
}

/* Ensure sequence cache has capacity up to n */
static void need_a_cache(idx_t n)
{
    if (n < a_cache_max) return;

    if (a_cache == NULL) {
        a_cache_cap = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = 1;
        a_cache_max = 0;
    }
    if (n + 1 > a_cache_cap) {
        a_cache_cap = n + 1;
        a_cache = (seq_t*)realloc(a_cache, a_cache_cap * sizeof(seq_t));
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    need_factorial(n);
    need_a_cache(n);

    /* Compute all missing terms up to n */
    for (idx_t m = a_cache_max + 1; m <= n; ++m) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        seq_t fact_m   = fact[m];

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term   = fact[k - 1];                /* (k-1)!   */
            seq_t denom  = fact[k] * fact[m - k];       /* k! * (m-k)! */
            seq_t binom  = fact_m / denom;             /* C(m,k)   */
            seq_t prod   = term * binom;               /* (k-1)!*C(m,k) */
            prod = prod * a_cache[m - k];              /* * a(m-k) */

            if (k & 1)      /* k odd → positive term */
                sum_pos += prod;
            else            /* k even → negative term */
                sum_neg += prod;
        }

        seq_t diff = sum_pos;
        if (sum_neg <= sum_pos) diff -= sum_neg;
        else diff = 0; /* safety against underflow */

        a_cache[m] = diff * 5;
    }

    a_cache_max = n;
    return a_cache[n];
}

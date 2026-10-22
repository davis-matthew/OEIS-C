/*
 * A368294  Expansion of e.g.f. exp(-x) / (1 - x*exp(2*x)).
 * https://oeis.org/A368294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*Sum_{k=0..n} (2*(n-k)-1)^k/k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent recurrence: a(0)=1; for n≥1,
   a(n)=(-1)^n + Sum_{k=0}^{n-1} binom(n,k)*a(k)*(n-k)*2^{n-k-1} ) */
#include <stdlib.h>

static seq_t *a_cache = NULL;     /* sequence values a(0)..a(n) */
static idx_t   a_cap   = 0;
static idx_t   a_size  = 0;

static seq_t *pow2_cache = NULL;  /* powers of 2: pow2[i] = 2^i */
static idx_t   pow2_cap   = 0;

/* Ensure that a_cache has room for indices 0..n inclusive */
static void ensure_a_capacity(idx_t n)
{
    if (n < a_cap) return;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) return; /* caller should handle allocation failure */
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Ensure that pow2_cache has room for indices 0..n inclusive */
static void ensure_pow2_capacity(idx_t n)
{
    if (n < pow2_cap) return;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(pow2_cache, new_cap * sizeof(seq_t));
    if (!tmp) return; /* caller should handle allocation failure */
    pow2_cache = tmp;
    /* compute new entries */
    for (idx_t i = pow2_cap; i <= n; ++i) {
        if (i == 0) pow2_cache[0] = (seq_t)1;
        else        pow2_cache[i] = pow2_cache[i-1] * (seq_t)2;
    }
    pow2_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1; /* a(0)=1 */

    ensure_a_capacity(n);
    ensure_pow2_capacity(n);          /* need pow2[0..n-1] */

    if (a_size == 0) {                /* initialize a(0) */
        a_cache[0] = (seq_t)1;
        a_size = 1;
    }

    for (idx_t m = a_size; m <= n; ++m) {
        if (m == 0) { a_cache[0] = (seq_t)1; continue; }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;                /* binom(m,0) */

        for (idx_t k = 0; k < m; ++k) {
            seq_t temp = a_cache[k];
            temp = temp * binom;                 /* a(k)*C(m,k) */
            temp = temp * (seq_t)(m - k);        /* *(m-k)       */
            temp = temp * pow2_cache[m - k - 1]; /* *2^{m-k-1}   */
            sum = sum + temp;

            if (k + 1 < m) {
                /* binom = binom * (m-k)/(k+1)  */
                binom = binom * (seq_t)(m - k);
                binom = binom / (seq_t)(k + 1);
            }
        }

        if (m & 1)          /* n odd: a(n)=sum-1 */
            a_cache[m] = sum - (seq_t)1;
        else                /* n even: a(n)=sum+1 */
            a_cache[m] = sum + (seq_t)1;
    }

    a_size = n + 1;
    return a_cache[n];
}

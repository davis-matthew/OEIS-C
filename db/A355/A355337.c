/*
 * A355337  Expansion of e.g.f.: exp(exp(x) + x^2 - 1).
 * https://oeis.org/A355337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; a(n) = 2*(n-1)*a(n-2) + Sum_{k=1..n}
 *            binomial(n-1,k-1)*a(n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache   = NULL;   /* cache of computed a(n) values   */
static size_t a_cap    = 0;        /* allocated capacity of a_cache  */
static size_t a_size   = 0;        /* number of values actually stored */

static seq_t *binom    = NULL;   /* binomial coefficients for current k  */
static size_t binom_cap = 0;     /* allocated capacity of binom array    */
static int    binom_k  = -1;      /* last k for which binom is correct   */

/* Ensure binomial array contains C(k, j) for 0 <= j <= k, where k is the
   requested value. The array grows and is updated using Pascal's rule.     */
static void ensure_binom(int k)
{
    if (binom_k >= k) return;

    if (binom == NULL) {
        size_t cap = 4;
        binom = (seq_t *)malloc(cap * sizeof(seq_t));
        binom_cap = cap;
        binom[0] = 1;          /* C(0,0) */
        binom_k = 0;
    }

    for (int cur = binom_k + 1; cur <= k; ++cur) {
        /* Resize array if necessary */
        if ((size_t)cur >= binom_cap) {
            size_t new_cap = binom_cap * 2;
            while (new_cap <= (size_t)cur + 1) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(binom, new_cap * sizeof(seq_t));
            binom = tmp;
            binom_cap = new_cap;
        }

        /* Update to next row of Pascal triangle */
        for (int j = cur; j >= 1; --j) {
            binom[j] = binom[j] + binom[j - 1];
        }
        /* binom[0] remains 1 */
        binom_k = cur;
    }
}

seq_t a(idx_t n)
{
    /* Ensure cache size is sufficient */
    if (a_cap <= n) {
        size_t new_cap = (a_cap == 0 ? 4 : a_cap * 2);
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cap = new_cap;
    }

    /* Initial values */
    if (a_size == 0) {
        a_cache[0] = 1;
        a_cache[1] = 1;
        a_size = 2;
    }

    /* Compute up to n */
    for (idx_t i = a_size; i <= n; ++i) {
        if (i <= 1) continue; /* a(0) and a(1) already set */

        /* Ensure binomial array for k = i-1 */
        ensure_binom((int)(i - 1));

        /* Compute sum_{j=0}^{i-1} C(i-1, j) * a[j] */
        seq_t sum = 0;
        int k = (int)(i - 1);
        for (int j = 0; j <= k; ++j) {
            sum += binom[j] * a_cache[j];
        }

        /* a(i) = 2*(i-1)*a(i-2) + sum */
        seq_t term = (seq_t)2 * (seq_t)(i - 1) * a_cache[i - 2];
        a_cache[i] = term + sum;
    }

    a_size = n + 1;
    return a_cache[n];
}

/*
 * A240921  G.f.: Sum_{n>=0} n^n * x^n * (1 + n*x)^n / (1 + n*x +
 *          n^2*x^2)^(n+1).
 * https://oeis.org/A240921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (n-k)! · S(n,n-k) · C(n-k,k) where S(n,m)
 *            are Stirling numbers of the second kind
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for memoisation */
static seq_t *cache   = NULL; /* a[0..]     */
static size_t cache_cap = 0;
static idx_t  cache_len = 0;   /* number of entries currently stored (>=1) */

static seq_t *facc   = NULL;   /* factorials   0.. */
static size_t facc_cap = 0;
static idx_t  cur_fac_n = 0;

static seq_t *Srow   = NULL;   /* Stirling row for current n */
static size_t Srow_cap = 0;
static idx_t  cur_stir_n = 0;

/* Grow an array of seq_t to accommodate at least 'needed' elements. */
static void grow_seq_t_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap) ? (*cap) * 2 : 8;
    if (newcap < needed) newcap = needed;
    *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Ensure all tables are prepared up to n. */
static void ensure_computed(idx_t n)
{
    /* Initialisation on first call */
    if (cache_len == 0) {
        /* cache[0] = 1 */
        grow_seq_t_array(&cache, &cache_cap, 1);
        cache[0] = (seq_t)1;
        cache_len = 1;

        /* factorial[0] = 1 */
        grow_seq_t_array(&facc, &facc_cap, 1);
        facc[0] = (seq_t)1;
        cur_fac_n = 0;

        /* Stirling row for n = 0 : S(0,0) = 1 */
        grow_seq_t_array(&Srow, &Srow_cap, 1);
        Srow[0] = (seq_t)1;
        cur_stir_n = 0;
    }

    if (n < cache_len) return;            /* already available */

    /* Grow cache to hold indices up to n */
    grow_seq_t_array(&cache, &cache_cap, n + 1);

    /* Iterate from current_n+1 to n */
    for (idx_t next = cur_stir_n + 1; next <= n; ++next) {
        /* Update factorial: fact[next] = fact[next-1] * next */
        grow_seq_t_array(&facc, &facc_cap, next + 1);
        facc[next] = facc[next - 1] * (seq_t)next;
        cur_fac_n = next;

        /* Extend Stirling array to accommodate next+1 entries */
        grow_seq_t_array(&Srow, &Srow_cap, next + 1);
        Srow[next] = (seq_t)0;          /* new element for m=next begins 0 */

        /* Update Stirling numbers using recurrence
           S(n,m) = m*S(n-1,m) + S(n-1,m-1) */
        for (idx_t m = next; m >= 1; --m) {
            Srow[m] = Srow[m] * (seq_t)m + Srow[m - 1];
        }
        Srow[0] = 0;                     /* S(next,0) = 0 for n>0 */

        /* Compute a(next) = Σ_{k=0..⌊next/2⌋} (next-k)! * S(next,next-k) * C(next-k,k) */
        idx_t kmax = next / 2;
        seq_t sum = (seq_t)0;

        for (idx_t k = 0; k <= kmax; ++k) {
            /* (next-k)! */
            seq_t term = facc[next - k];
            /* * S(next, next-k) */
            term = term * Srow[next - k];

            /* Compute binomial C(next-k, k) via multiplicative formula */
            seq_t comb = (seq_t)1;
            idx_t n1 = next - k;
            for (idx_t i = 1; i <= k; ++i) {
                comb = comb * (seq_t)(n1 - i + 1) / (seq_t)i;
            }
            /* * comb */
            term = term * comb;

            sum += term;
        }
        cache[next] = sum;   /* cache the new term */
    }

    cur_stir_n = n;
    cache_len = n + 1;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}

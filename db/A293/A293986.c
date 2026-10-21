/*
 * A293986  Expansion of e.g.f. exp((1/(1-x)^7 - 1)/7).
 * https://oeis.org/A293986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Expansion of e.g.f. exp((1/(1-x)^7 - 1)/7)
   a(n) – coefficients of the exponential generating function
   a(0) = 1, a(1) = 1, a(2) = 9, a(3) = 97, ...
   Recurrence:
     For n ≥ 1
        a[n] = a[n‑1] + Σ_{j=1}^{min(8,n‑1)}  
                 (-1)^{j+1} * binom(8,j) *
                 a[n‑j] * Π_{t=n‑j}^{n‑1} t
*/
#include <stddef.h>

/* binomial coefficients C(8,j) for j=0..8 */
static const int binom[9] = {1,8,28,56,70,56,28,8,1};

/* small cache for 0..MAXN terms (should cover n up to about 10000) */
#define MAXN 10005
static seq_t cache[MAXN];
static idx_t cache_len = 0;

/* Compute a[n] using the recurrence above, expanding the cache as needed */
seq_t a(idx_t n)
{
    /* initialize cache[0] = 1, cache[1] = 1 */
    if (cache_len == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    /* extend cache up to n */
    while (cache_len <= n) {
        idx_t m = cache_len - 1;        /* m = n-1 in recurrence */
        seq_t next = cache[m];          /* start with a[m] */

        /* accumulate contributions from j = 1 .. min(8,m) */
        for (idx_t j = 1; j <= 8 && j <= m; ++j) {
            /* term = a[m-j+1] * binom(8,j) * product_{t=m-j+1}^{m} t */
            seq_t term = cache[m - j + 1];
            term *= (seq_t)binom[j];

            /* multiply by the product of j consecutive integers */
            idx_t t;
            for (t = m - j + 1; t <= m; ++t)
                term *= (seq_t)t;

            /* add or subtract depending on parity of j */
            if (j & 1)
                next += term;          /* + for odd j   */
            else
                next -= term;          /* - for even j  */
        }

        cache[cache_len++] = next;
    }

    return cache[n];
}

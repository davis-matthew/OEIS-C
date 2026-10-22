/*
 * A397859  G.f. A(x) satisfies A(x) = 1 + x * (1+x) * A(x * (1+x)^2).
 * https://oeis.org/A397859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n-1} binomial(2*k+1, n-k-1) * a(k), a(0)=1
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility: ensure cache is large enough */
static void ensure_cache(idx_t n, seq_t **ptr, size_t *cap)
{
    if (n < *cap) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap * 2;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *ptr = tmp;
    *cap = new_cap;
}

/* Compute binomial(a,b) for non‑negative a,b using multiplicative formula.
 * The result is exact because the division is integral at each step. */
static seq_t binom_small(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;           /* use symmetry to reduce work */
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * ((seq_t)(a - b + i)) / (idx_t)i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t size = 0;

    /* Ensure cache is large enough for n */
    ensure_cache(n, &cache, &cap);
    if (size <= n) {
        /* Compute missing terms up to n */
        for (idx_t m = size; m <= n; ++m) {
            seq_t sum = 0;
            if (m == 0) {
                sum = 1;                     /* a(0)=1 */
            } else {
                for (idx_t k = 0; k < m; ++k) {
                    idx_t r = m - k - 1;      /* n-k-1 */
                    if (r > 2 * k + 1) continue;   /* binomial is zero */
                    seq_t b = binom_small(2 * k + 1, r);
                    sum += b * cache[k];
                }
            }
            cache[m] = sum;
        }
        size = n + 1;
    }
    return cache[n];
}

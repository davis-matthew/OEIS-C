/*
 * A377692  E.g.f. satisfies A(x) = (1 - log(1 - x) * A(x))^2.
 * https://oeis.org/A377692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=0..n} ((2*k+1)!/(k+2)!) * |Stirling1(n,k)|
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static caches for computed terms and helper arrays. */
static seq_t *a_cache  = NULL;       /* a(0..max_n)   */
static seq_t *t_cache  = NULL;       /* t_k = 2*(2k+1)!/(k+2)!  */
static seq_t *s_prev   = NULL;       /* previous row of |Stirling1| */
static seq_t *s_curr   = NULL;       /* current row buffer */
static idx_t   max_n   = 0;
static size_t  cache_cap = 0;
static size_t  t_cap     = 0;
static size_t  sp_cap    = 0;

/* Ensure the cache arrays are sized to at least size elements. */
static void ensure_cache_capacity(size_t size)
{
    if (cache_cap < size) {
        size_t new_cap = size;
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }
}

static void ensure_t_capacity(size_t size)
{
    if (t_cap < size) {
        size_t new_cap = size;
        t_cache = (seq_t *)realloc(t_cache, sizeof(seq_t) * new_cap);
        t_cap = new_cap;
    }
}

static void ensure_sp_capacity(size_t size)
{
    if (sp_cap < size) {
        size_t new_cap = size;
        s_prev = (seq_t *)realloc(s_prev, sizeof(seq_t) * new_cap);
        s_curr = (seq_t *)realloc(s_curr, sizeof(seq_t) * new_cap);
        sp_cap = new_cap;
    }
}

/* Return a(n), with memoisation. */
seq_t a(idx_t n)
{
    /* Initialise caches on first call. */
    if (a_cache == NULL) {
        cache_cap = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        a_cache[0] = (seq_t)1;               /* a(0) */

        t_cap = 1;
        t_cache = (seq_t *)malloc(sizeof(seq_t) * t_cap);
        t_cache[0] = (seq_t)1;               /* t_0 = 1 */

        sp_cap = 1;
        s_prev = (seq_t *)malloc(sizeof(seq_t) * sp_cap);
        s_curr = (seq_t *)malloc(sizeof(seq_t) * sp_cap);
        s_prev[0] = (seq_t)1;               /* Stirling(0,0) = 1 */

        max_n = 0;
    }

    /* Return cached value if already computed. */
    if (n <= max_n) {
        return a_cache[n];
    }

    /* Increase capacities if needed. */
    ensure_cache_capacity((size_t)n + 1);
    ensure_t_capacity((size_t)n + 1);
    ensure_sp_capacity((size_t)n + 1);

    /* Compute t_k for k = max_n+1 .. n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t prev = t_cache[k - 1];
        seq_t mult = ((seq_t)(2 * k + 1)) * ((seq_t)(2 * k));
        seq_t next = prev * mult;
        next = next / ((seq_t)(k + 2));
        t_cache[k] = next;
    }

    /* Extend a_cache and compute a(m) for m = max_n+1 .. n. */
    for (idx_t m = max_n + 1; m <= n; ++m) {

        /* Compute row m of |Stirling1| into s_curr using s_prev. */
        for (idx_t k = 0; k <= m; ++k) {
            seq_t part1 = 0, part2 = 0;
            if (k > 0)       part1 = s_prev[k - 1];
            if (k <= m - 1)  part2 = ((seq_t)(m - 1)) * s_prev[k];
            s_curr[k] = part1 + part2;
        }

        /* Compute a(m) = Σ_{k=0..m} t_k * s_curr[k]. */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k) {
            sum += t_cache[k] * s_curr[k];
        }
        a_cache[m] = sum;

        /* Prepare for next iteration: swap rows. */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }

    max_n = n;
    return a_cache[n];
}

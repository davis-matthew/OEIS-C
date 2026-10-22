/*
 * A373816  Expansion of 1/(2 - 1/(1 - 4*x)^(3/2)).
 * https://oeis.org/A373816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * Σ_{k>=0} (1/2)^(k+1) * binomial(n-1 + 3*k/2, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static storage for memoisation */
static seq_t *a_cache = NULL;     /* a[0..a_used-1]    */
static seq_t *g_cache = NULL;     /* g[0..g_used-1]    */
static size_t a_cap = 0;          /* capacity of a_cache   */
static size_t g_cap = 0;          /* capacity of g_cache   */
static idx_t a_used = 0;          /* next free index of a  */
static idx_t g_used = 0;          /* next free index of g  */

/* Ensure a_cache[0..n] and g_cache[0..n] are available and filled.  */
static void
ensure_cache(idx_t n)
{
    /* ---------- grow and initialise g_cache ---------- */
    if (g_used == 0) {
        /* allocate minimal capacity */
        size_t new_cap = 1;
        g_cache = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        g_cap = new_cap;
        /* g[0] = 1 */
        g_cache[0] = (seq_t)1;
        g_used = 1;
    }
    if (g_cap <= (size_t)n) {
        size_t new_cap = g_cap ? g_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        g_cache = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        g_cap = new_cap;
    }
    /* compute g[k] for k = g_used .. n */
    for (idx_t k = g_used; k <= n; ++k) {
        if (k == 0) continue; /* already set */
        /* g_k = g_{k-1} * (4*k + 2) / k   (exact division) */
        seq_t num = g_cache[k - 1] * ((seq_t)(4 * k + 2));
        g_cache[k] = num / (seq_t)k;  /* division is exact */
    }
    g_used = n + 1;

    /* ---------- grow and initialise a_cache ---------- */
    if (a_used == 0) {
        size_t new_cap = 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
        a_cache[0] = (seq_t)1;  /* a[0] = 1 */
        a_used = 1;
    }
    if (a_cap <= (size_t)n) {
        size_t new_cap = a_cap ? a_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* compute a[m] for m = a_used .. n  */
    for (idx_t m = a_used; m <= n; ++m) {
        seq_t sum = 0;
        /* a_m = Σ_{k=1}^{m} g_k * a_{m-k}   (derived from (2-G(x))F(x)=1) */
        for (idx_t k = 1; k <= m; ++k) {
            sum += g_cache[k] * a_cache[m - k];
        }
        a_cache[m] = sum;
    }
    a_used = n + 1;
}

/* Exported function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}

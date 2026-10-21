/*
 * A254653  Indices of centered heptagonal numbers (A069099) which are also
 *          pentagonal numbers (A000326).
 * https://oeis.org/A254653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+110*a(n-2)-110*a(n-3)-a(n-4)+a(n-5) Initial values
 *            (n : a(n)): 1 : 1 2 : 3 3 : 58 4 : 276 5 : 6325
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms currently stored */
static idx_t cache_cap = 0;   /* allocated capacity (in terms of seq_t) */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)needed * sizeof(seq_t));
    cache = tmp;
    cache_cap = needed;
}

/* Initialise the first five terms of the sequence. */
static void init_initial(void)
{
    if (cache_len != 0) return;
    ensure_capacity(5);
    cache[0] = 1;           /* a(1) */
    cache[1] = 3;           /* a(2) */
    cache[2] = 58;          /* a(3) */
    cache[3] = 276;         /* a(4) */
    cache[4] = 6325;        /* a(5) */
    cache_len = 5;
}

/* Compute all terms up to and including index n (1‑based). */
static void compute_up_to(idx_t n)
{
    init_initial();
    if (n <= cache_len) return;

    ensure_capacity(n);  /* make sure we have space for all terms up to n */

    for (idx_t k = cache_len + 1; k <= n; ++k) {
        idx_t idx = k - 1;                      /* array index of a(k) */
        seq_t val = cache[idx - 1]              /* a(k-1) */
                  + (seq_t)110 * cache[idx - 2] /* 110*a(k-2) */
                  - (seq_t)110 * cache[idx - 3]/* -110*a(k-3) */
                  - cache[idx - 4]             /* -a(k-4)   */
                  + cache[idx - 5];            /* +a(k-5)   */

        cache[idx] = val;
    }
    cache_len = n;
}

/* Public entry point: return a(n) for n ≥ 1 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset < 1 */
    compute_up_to(n);
    return cache[n - 1];
}

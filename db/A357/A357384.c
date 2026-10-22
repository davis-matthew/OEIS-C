/*
 * A357384  Expansion of 1 / (1 + Sum_{k>=1}(-x)^Lucas(k)).
 * https://oeis.org/A357384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 39, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = –∑_{L≤n, L Lucas} (–1)^L a(n–L),  a(0)=1.
   Here the Lucas numbers are L_0=2, L_1=1, L_{k+2}=L_{k+1}+L_k;
   the recurrence follows from (1+∑_{k≥1}(-x)^{L_k})·G(x)=1.  */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <new>
#endif

/*--- Lucas numbers ------------------------------------------------------*/
static idx_t *lucas_arr = NULL;       /* stored Lucas numbers (starting with L_1=1) */
static size_t lucas_cap = 0;
static size_t lucas_len = 0;
static idx_t lucas_prev = 2;          /* L_0 */
static idx_t lucas_curr = 1;          /* L_1 */

/* Grow lucas_arr capacity.  */
static void lucas_expand(void)
{
    size_t new_cap = lucas_cap ? lucas_cap * 2 : 8;
    idx_t *tmp = (idx_t*)realloc(lucas_arr, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    lucas_arr = tmp;
    lucas_cap = new_cap;
}

/* Make lucas_arr contain all Lucas numbers ≤ limit (starting from L_1). */
static void ensure_lucas_upto(idx_t limit)
{
    while (lucas_curr <= limit)
    {
        if (lucas_len == lucas_cap) lucas_expand();
        lucas_arr[lucas_len++] = lucas_curr;
        idx_t next = lucas_prev + lucas_curr;
        lucas_prev = lucas_curr;
        lucas_curr = next;
    }
}

/*--- Cache for a(n) -----------------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = (idx_t)-1;   /* sentinel meaning not initialised */

/* Resize the cache to new_cap (must be > current capacity). */
static void seq_cache_resize(size_t new_cap)
{
    if (new_cap <= cache_cap) return;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
    delete [] cache;
#else
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/*--- Sequence function --------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache_max == (idx_t)-1) {
        cache_max = 0;
        seq_cache_resize(1);
        cache[0] = 1;   /* a(0) = 1 */
    }

    /* Return cached value if available. */
    if (n <= cache_max) return cache[(size_t)n];

    /* Grow cache if necessary. */
    size_t needed_cap = ((size_t)n + 1) * 2;
    if (needed_cap > cache_cap) seq_cache_resize(needed_cap);

    /* Ensure Lucas numbers up to n are ready. */
    ensure_lucas_upto(n);

    /* Compute the sequence from cache_max+1 up to n. */
    for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
        seq_t sum_odd = 0;
        seq_t sum_even = 0;

        for (size_t j = 0; j < lucas_len && lucas_arr[j] <= cur; ++j) {
            idx_t L = lucas_arr[j];
            idx_t shift = cur - L;
            seq_t prev = cache[(size_t)shift];

            if (L & 1)          /* odd Lucas contributes positively */
                sum_odd += prev;
            else
                sum_even += prev;   /* even Lucas contributes negatively */
        }

        cache[(size_t)cur] = sum_odd - sum_even;
    }

    cache_max = n;
    return cache[(size_t)n];
}

/*
 * A297335  Number of n X 5 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n‑1) - 40*a(n‑2) - 12*a(n‑3) + 107*a(n‑4) -
 *            12*a(n‑5) - 60*a(n‑6) + 16*a(n‑7) Initial values: a(1)=13,
 *            a(2)=134, a(3)=1310, a(4)=13067, a(5)=130297, a(6)=1299649,
 *            a(7)=12964224, a(8)=129324245
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cached sequence values          */
static idx_t cache_len = 0;           /* number of terms currently cached */

/* Initialize the first eight terms of the sequence. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_len = 8;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (!cache)
        exit(1);

    cache[0] = (seq_t)13ULL;
    cache[1] = (seq_t)134ULL;
    cache[2] = (seq_t)1310ULL;
    cache[3] = (seq_t)13067ULL;
    cache[4] = (seq_t)130297ULL;
    cache[5] = (seq_t)1299649ULL;
    cache[6] = (seq_t)12964224ULL;
    cache[7] = (seq_t)129324245ULL;
}

/* Return the nth term of sequence A297335 (1‑based).  The function may be
 * called with n in any order; it memoises values so each term is computed
 * only once. */
seq_t a(idx_t n)
{
    init_cache();

    /* Return already cached value if available. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Resize cache to hold up to term n. */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache)
        exit(1);
    cache = new_cache;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* indices are 1‑based in the recurrence. */
        seq_t val =
            (seq_t)14   * cache[i - 2]    /* a(i‑1) */
          - (seq_t)40   * cache[i - 3]    /* a(i‑2) */
          - (seq_t)12   * cache[i - 4]    /* a(i‑3) */
          + (seq_t)107  * cache[i - 5]    /* a(i‑4) */
          - (seq_t)12   * cache[i - 6]    /* a(i‑5) */
          - (seq_t)60   * cache[i - 7]    /* a(i‑6) */
          + (seq_t)16   * cache[i - 8];   /* a(i‑7) */

        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}

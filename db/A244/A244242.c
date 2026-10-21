/*
 * A244242  Number of partitions of n into 6 parts such that every i-th
 *          smallest part (counted with multiplicity) is different from i.
 * https://oeis.org/A244242
 *
 * Offset:    27    (a(27) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 92, u32 n <= 821, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) - a(n-5) - 2*a(n-7) + a(n-9) + a(n-10) +
 * a(n-11) + a(n-12) - 2*a(n-14) - a(n-16) + a(n-19) + a(n-20) - a(n-21)
 *   for n > 57.
 * Initial values a(27)…a(57) are computed by a depth–first search over all
 * non‑decreasing 6‑tuples that satisfy ai≠i.
 * The implementation uses a cache to evaluate values in any order.
 */

#include <stdlib.h>

/* ------------------------------------------------------------ */
/* -----  Depth‑first enumeration for small n  ---------------- */

static void dfs(idx_t depth, idx_t start, idx_t sum_left, int *cnt)
{
    /* depth = 1 … 6; start = lower bound for the next part;
       sum_left = remaining sum to distribute.
       cnt holds the total number of valid partitions.           */
    if (depth == 6) {
        /* last part is forced to sum_left */
        if (sum_left >= start && sum_left != 6) {
            (*cnt)++;
        }
        return;
    }

    /* maximum value that allows at least one value for each remaining part: */
    idx_t max_val = sum_left / (6 - depth + 1);
    for (idx_t v = start; v <= max_val; ++v) {
        if (v == depth) continue;   /* ai ≠ i restriction */
        dfs(depth + 1, v, sum_left - v, cnt);
    }
}

/* Compute the number of admissible partitions for a given n (n ≤ 57). */
static seq_t compute_partition_count(idx_t n)
{
    int cnt = 0;
    dfs(1, 1, n, &cnt);
    return (seq_t)cnt;
}

/* ------------------------------------------------------------ */
/* -----  Memoised cache of all computed values --------------- */

static seq_t *cache          = NULL;   /* dynamic array of values */
static size_t cache_capacity = 0;      /* current capacity (number of slots) */
static const idx_t OFFSET    = 27;      /* first term of the sequence */

/* Ensure the cache is large enough for indices up to n,
   and initialise any new entries. */
static void ensure_cache(idx_t n)
{
    if (n < OFFSET) {
        return;                      /* values below the offset are irrelevant */
    }

    if ((idx_t)(cache_capacity) > n) {
        return;                      /* already computed up to n */
    }

    size_t new_capacity = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failed */

    cache = tmp;

    /* Fill new entries. */
    for (idx_t i = (idx_t)cache_capacity; i <= n; ++i) {
        if (i < OFFSET) {
            cache[i] = (seq_t)0;       /* behaviour below the offset is undefined */
        } else if (i <= 57) {
            cache[i] = compute_partition_count(i);
        } else {
            /* Recurrence for n > 57 */
            seq_t pos = cache[i-1] + cache[i-2];
            pos += cache[i-9] + cache[i-10] + cache[i-11] + cache[i-12];
            pos += cache[i-19] + cache[i-20];

            seq_t neg = cache[i-5];
            neg += cache[i-7] + cache[i-7];
            neg += cache[i-14] + cache[i-14];
            neg += cache[i-16];
            neg += cache[i-21];

            cache[i] = pos - neg;
        }
    }

    cache_capacity = new_capacity;
}

/* ------------------------------------------------------------ */
/* -----  Public interface: sequence a(n) --------------------- */

seq_t a(idx_t n)
{
    if (n < OFFSET) {
        return (seq_t)0;          /* undefined below the offset */
    }
    ensure_cache(n);
    return cache[n];
}

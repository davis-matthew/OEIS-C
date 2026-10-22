/*
 * A308769  Sum of the second largest parts of the partitions of n into 4
 *          squarefree parts.
 * https://oeis.org/A308769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/4)} Sum_{j=k..floor((n-k)/3)}
 *            Sum_{i=j..floor((n-j-k)/2)} mu(k)^2 * mu(j)^2 * mu(i)^2 *
 *            mu(n-i-j-k)^2 * i (counts the second largest part of all
 *            partitions of n into 4 square‑free parts)
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/* Static tables used by the cache. The tables are rebuilt whenever a larger
 * n is requested. All pointers start as NULL. */
static uint8_t  *sqfree_buf      = NULL;          /* 1 if i is square‑free */
static uint16_t *sf_list        = NULL;          /* list of all square‑free numbers ≤ n */
static size_t   sf_count        = 0;              /* number of items in sf_list */

static size_t  *pair_count     = NULL;          /* number of unordered pairs (k,j) with k≤j, k+j = rem */
static size_t  *pair_offset    = NULL;          /* start index in pair_data for each rem */
static uint16_t *pair_data      = NULL;          /* concatenated j-values of all pairs, sorted ascending */

static idx_t    cached_max_n    = 0;              /* maximum n covered by the tables */

/* -------------------------------------------------------------------------- */
/* Rebuild the static tables so that the maximum n handled is at least n. */
static void ensure_cache(idx_t n)
{
    if (n <= cached_max_n)
        return;   /* already ready */

    /* free old tables if any */
    free(sqfree_buf);
    free(sf_list);
    free(pair_count);
    free(pair_offset);
    free(pair_data);

    /* 1. Build square‑free bitmask up to n. */
    sqfree_buf = (uint8_t *)malloc((size_t)(n + 1));
    if (!sqfree_buf)
        return;   /* allocation failure */

    for (idx_t i = 1; i <= n; ++i)
        sqfree_buf[i] = 1;        /* assume square‑free */
    sqfree_buf[0] = 0;            /* 0 never used */

    for (idx_t d = 2; d * d <= n; ++d)
        if (sqfree_buf[d])         /* need to process composites too */
            for (idx_t m = d * d; m <= n; m += d * d)
                sqfree_buf[m] = 0;  /* not square‑free */

    /* 2. Gather all square‑free numbers into sf_list. */
    sf_count = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (sqfree_buf[i])
            ++sf_count;

    sf_list = (uint16_t *)malloc(sf_count * sizeof(uint16_t));
    if (!sf_list)
        return;

    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (sqfree_buf[i])
            sf_list[pos++] = (uint16_t)i;

    /* 3. Pre‑compute pair statistics for all sums up to n. */
    pair_count  = (size_t *)malloc((size_t)(n + 1) * sizeof(size_t));
    pair_offset = (size_t *)malloc((size_t)(n + 1) * sizeof(size_t));
    if (!pair_count || !pair_offset)
        return;

    size_t total_pairs = 0;
    for (idx_t rem = 2; rem <= n; ++rem) {
        size_t cnt = 0;
        for (idx_t k = rem / 2; k >= 1; --k) {
            idx_t j = rem - k;
            if (sqfree_buf[k] && sqfree_buf[j])
                ++cnt;
        }
        pair_count[rem]  = cnt;
        pair_offset[rem] = total_pairs;
        total_pairs     += cnt;
    }

    /* 4. Store the j-values of all pairs, sorted ascending for each sum. */
    pair_data = (uint16_t *)malloc(total_pairs * sizeof(uint16_t));
    if (!pair_data)
        return;

    for (idx_t rem = 2; rem <= n; ++rem) {
        size_t cur = pair_offset[rem];
        for (idx_t k = rem / 2; k >= 1; --k) {
            idx_t j = rem - k;
            if (sqfree_buf[k] && sqfree_buf[j])
                pair_data[cur++] = (uint16_t)j;   /* j ascending because k decreases */
        }
    }

    cached_max_n = n;      /* tables now valid up to n */
}

/* -------------------------------------------------------------------------- */
/* Return the value of A308769 for the given n (0‑based). */
seq_t a(idx_t n)
{
    if (n < 4)          /* no partition into 4 parts possible */
        return 0;

    ensure_cache(n);

    unsigned long long acc = 0;      /* intermediate accumulator */

    /* i (second largest part) must be square‑free and ≤ n/2 */
    idx_t max_i = n / 2;
    for (size_t idx_i = 0; idx_i < sf_count && (idx_t)sf_list[idx_i] <= max_i; ++idx_i) {
        idx_t i = sf_list[idx_i];    /* i is square‑free */

        /* Iterate over l (largest part) ≥ i, also square‑free.
           Since sqfree list is sorted, we can start at idx_i. */
        for (size_t idx_l = idx_i; idx_l < sf_count; ++idx_l) {
            idx_t l = sf_list[idx_l];      /* l is square‑free and l ≥ i */

            /* remainder for the two smaller parts */
            if (l > n - i - 2)    /* rem would become < 2 */
                break;

            idx_t rem = n - i - l;       /* rem = k + j, must be ≥ 2 */

            if (rem < 2)
                break;
            if (rem > 2 * i)     /* j + k must be ≤ 2i to keep l ≥ i */
                continue;

            size_t cnt = pair_count[rem];
            if (cnt == 0)
                continue;

            /* Find number of pairs with j ≤ i. pair_data for rem is sorted by j. */
            size_t lo = 0, hi = cnt;
            size_t base = pair_offset[rem];
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (pair_data[base + mid] <= (uint16_t)i)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            /* lo is the count of valid pairs for this rem. */
            acc += (unsigned long long)i * (unsigned long long)lo;
        }
    }

    return (seq_t)acc;
}

/*
 * A339273  Sums of two nonzero even squares.
 * https://oeis.org/A339273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(i² + j²) over all positive integers i, j, sorted
 *            unique
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* Comparator for qsort, works for any unsigned integer seq_t. */
static int seq_cmp(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Integer square root for idx_t (assumes idx_t is unsigned). */
static idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t r = (idx_t) sqrt((double) n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* Returns the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Static storage for sequence and helper arrays. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint8_t *seen = NULL;
    static size_t seen_cap = 0;
    static idx_t current_limit = 0;   /* Largest sum examined so far */

    if (n == 0) return 0;   /* Undefined for offset 1, but safe return. */

    /* Grow the cache until we have at least n terms. */
    while (terms_len < n) {
        /* Increase the search bound. */
        idx_t new_limit = (current_limit == 0) ? 16 : current_limit * 2;
        if (new_limit < 4) new_limit = 4;

        /* Ensure seen array can index all sums up to new_limit. */
        if (seen_cap <= (size_t)new_limit) {
            size_t new_cap = seen_cap == 0 ? new_limit + 1 : seen_cap * 2;
            if (new_cap <= (size_t)new_limit) new_cap = new_limit + 1;
            uint8_t *tmp = (uint8_t *)realloc(seen, new_cap);
            if (!tmp) abort();
            /* Zero newly allocated part. */
            memset(tmp + seen_cap, 0, new_cap - seen_cap);
            seen = tmp;
            seen_cap = new_cap;
        }

        /* Generate all sums 4*(i² + j²) with i, j ≥ 1 and sum ≤ new_limit. */
        idx_t S_lim = new_limit / 4;
        idx_t i_max = isqrt_u64(S_lim);
        for (idx_t i = 1; i <= i_max; ++i) {
            idx_t i2 = i * i;
            idx_t remain = S_lim - i2;
            if (remain < i2) continue;   /* ensures j ≥ i */
            idx_t j_max = isqrt_u64(remain);
            for (idx_t j = i; j <= j_max; ++j) {
                idx_t sum_idx = 4 * (i2 + j * j);
                if (sum_idx > new_limit) continue;
                if (!seen[sum_idx]) {
                    seen[sum_idx] = 1;
                    /* Append to terms array. */
                    if (terms_len >= terms_cap) {
                        size_t new_cap = terms_cap == 0 ? 64 : terms_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        terms = tmp;
                        terms_cap = new_cap;
                    }
                    terms[terms_len++] = (seq_t)sum_idx;
                }
            }
        }

        /* Keep the terms sorted for correct indexing. */
        qsort(terms, terms_len, sizeof(seq_t), seq_cmp);

        current_limit = new_limit;
    }

    return terms[n - 1];
}

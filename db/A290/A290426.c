/*
 * A290426  Products of two self numbers (A003052).
 * https://oeis.org/A290426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Products of two self numbers (A003052)
 * Range:     u8 none, u16 none, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth smallest product of two self numbers (with repeated
 * self numbers allowed).  Self numbers are numbers that are not the sum
 * of any number and the sum of its digits. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compare function for qsort on seq_t values. */
static int seq_cmp(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x > y) - (x < y);
}

/* Auxiliary: digit sum of an idx_t number. */
static inline idx_t digit_sum(idx_t v)
{
    idx_t s = 0;
    while (v) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

/* Static state: cached terms, count and the limit that was used. */
static seq_t *cached_terms = NULL;
static size_t cached_count = 0;
static idx_t cached_limit = 0;

/* Recompute the sequence up to at least n terms. */
static void compute_terms(idx_t n)
{
    idx_t limit = cached_limit ? cached_limit : 1000;

    /* Grow limit until we have enough terms. */
    while (1) {
        /* 1. Find all self numbers ≤ limit. */
        bool *marked = (bool *)calloc((size_t)(limit + 1), 1);
        if (!marked)
            abort();

        for (idx_t i = 1; i <= limit; ++i) {
            idx_t d = i + digit_sum(i);
            if (d <= limit)
                marked[d] = true;
        }

        /* Collect self numbers. */
        size_t self_cap = (size_t)limit + 1; /* worst case */
        idx_t *self = (idx_t *)malloc(self_cap * sizeof(idx_t));
        size_t self_cnt = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (!marked[i])
                self[self_cnt++] = i;
        }
        free(marked);

        /* 2. Generate all products self[i] * self[j] with i ≤ j. */
        size_t prod_cap = self_cnt * (self_cnt + 1) / 2;
        seq_t *prods = (seq_t *)malloc(prod_cap * sizeof(seq_t));
        size_t prod_cnt = 0;

        for (size_t i = 0; i < self_cnt; ++i) {
            seq_t a = (seq_t)self[i];
            for (size_t j = i; j < self_cnt; ++j) {
                seq_t b = (seq_t)self[j];
                prods[prod_cnt++] = a * b;
            }
        }
        free(self);

        /* 3. Sort and deduplicate. */
        qsort(prods, prod_cnt, sizeof(seq_t), seq_cmp);

        size_t uniq_cnt = 0;
        for (size_t i = 0; i < prod_cnt; ++i) {
            if (i == 0 || prods[i] != prods[i - 1]) {
                prods[uniq_cnt++] = prods[i];
            }
        }

        if (uniq_cnt >= (size_t)n) {
            /* Store the results. */
            free(cached_terms);
            cached_terms = prods;
            cached_count = uniq_cnt;
            cached_limit = limit;
            break;
        } else {
            /* Not enough terms – enlarge the limit and try again. */
            free(prods);
            limit *= 2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; return 0 as a fallback */

    if (!cached_terms || n > cached_count)
        compute_terms(n);

    return cached_terms[n - 1];
}

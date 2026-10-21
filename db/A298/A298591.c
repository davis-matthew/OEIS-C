/*
 * A298591  Numbers which are the sum of two distinct perfect powers x^k + y^m
 *          with x, y, k, m >= 2.
 * https://oeis.org/A298591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest positive integer that can be expressed *
 *            as a sum of two distinct perfect powers x^k + y^m with x,y,k,m ≥
 *            2. * The function lazily builds a table of these sums up to a
 *            fixed limit * that is sufficient to cover ≥ 20000 terms, then
 *            answers any query * in O(1). The table is r...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_cmp.h"     /* seq_cmp */

#define TARGET_TERMS (20000U)         /* terms we guarantee to pre‑compute   */
#define INITIAL_LIMIT ((seq_t)10000000ULL) /* starting upper bound for sums      */

static int seq_cmp_wrapper(const void *a, const void *b)
{
    return seq_cmp(a, b);
}

/* Ensure a resizable vector has enough capacity. */
static void grow_vector(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t new_cap = (*cap) << 1;
        if (new_cap < need)
            new_cap = need;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Build the table of sums up to the given limit.  Returns the number of
 * distinct sums found.  The resulting array is stored in *out (must be
 * freed by the caller). */
static size_t build_sums(seq_t limit, seq_t **out)
{
    /* ---- Build all perfect powers ≤ limit ---- */
    size_t pw_cap = 256;
    seq_t *powers = (seq_t *)malloc(pw_cap * sizeof(seq_t));
    size_t pw_cnt = 0;

    for (seq_t base = 2; ; ++base) {
        /* base^2 must not overflow limit */
        if (base > limit / base)
            break;
        seq_t curr = base * base;           /* power = base^2 */
        /* store the first non‑trivial power */
        grow_vector(&powers, &pw_cap, pw_cnt + 1);
        powers[pw_cnt++] = curr;
        /* generate higher exponents: curr *= base until exceeding limit */
        while (curr <= limit / base) {
            curr *= base;
            grow_vector(&powers, &pw_cap, pw_cnt + 1);
            powers[pw_cnt++] = curr;
        }
    }

    /* sort and deduplicate powers */
    qsort(powers, pw_cnt, sizeof(seq_t), seq_cmp_wrapper);
    size_t uniq_pw = 0;
    for (size_t i = 0; i < pw_cnt; ++i) {
        if (i == 0 || seq_cmp(&powers[i], &powers[uniq_pw - 1]) != 0) {
            powers[uniq_pw++] = powers[i];
        }
    }
    pw_cnt = uniq_pw;

    /* ---- Generate all distinct sums pw[i] + pw[j] with i < j ---- */
    size_t sum_cap = 1 << 20;          /* start with 1,048,576 slots */
    seq_t *sums = (seq_t *)malloc(sum_cap * sizeof(seq_t));
    size_t sum_cnt = 0;

    for (size_t i = 0; i < pw_cnt; ++i) {
        for (size_t j = i + 1; j < pw_cnt; ++j) {
            /* guard against overflow: pw[i] > limit - pw[j] => sum > limit */
            if (powers[i] > limit - powers[j])
                continue;
            seq_t val = powers[i] + powers[j];
            grow_vector(&sums, &sum_cap, sum_cnt + 1);
            sums[sum_cnt++] = val;
        }
    }

    /* sort and deduplicate sums */
    qsort(sums, sum_cnt, sizeof(seq_t), seq_cmp_wrapper);
    size_t uniq_sum = 0;
    for (size_t i = 0; i < sum_cnt; ++i) {
        if (i == 0 || seq_cmp(&sums[i], &sums[uniq_sum - 1]) != 0) {
            sums[uniq_sum++] = sums[i];
        }
    }
    sum_cnt = uniq_sum;

    /* finalize output */
    *out = sums;
    free(powers);
    return sum_cnt;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    static seq_t *sequence = NULL;
    static size_t seq_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Build once with the default limit; increase if necessary. */
        seq_t limit = INITIAL_LIMIT;
        while (1) {
            sequence = NULL;
            seq_len = build_sums(limit, &sequence);
            if (seq_len >= TARGET_TERMS)
                break;
            /* Double the limit for the next iteration (prevent overflow). */
            if (limit == 0)
                break;
            limit <<= 1;
        }
        initialized = true;
    }

    /* n is 1‑based; caller guarantees n ≥ 1. */
    if ((idx_t) (n - 1) < seq_len)
        return sequence[(size_t)(n - 1)];
    return 0;   /* fallback – should not happen for supported n. */
}

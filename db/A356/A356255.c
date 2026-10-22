/*
 * A356255  a(1) = 1; for n > 1, a(n) is the smallest magnitude number not
 *          occurring earlier such that n is divisible by s = Sum_{k = 1..n}
 *          a(k), where |s| > 1.
 * https://oeis.org/A356255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(1) = 1; For n > 1, a(n) is the smallest magnitude number not
 *            occurring earlier such that n is divisible by s = Sum_{k=1..n}
 *            a(k), where |s| > 1. The function computes the terms
 *            sequentially and caches them for subsequent calls
 * Range:     i8 n <= 66, i16 n <= 2020, i32 n <= 2020, i64 n <= 2020,
 *            i128 n <= 2020, big any n
 * Helpers:   abs_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/abs_seq.h"

static seq_t *seq_cache = NULL;   /* cached terms */
static size_t seq_len  = 0;       /* number of terms cached */
static size_t seq_cap  = 0;       /* capacity of seq_cache */
static seq_t  seq_sum  = 0;       /* cumulative sum of cached terms */

/* Checks whether val occurs among the first 'upto' cached terms. */
static bool used_in_cache(seq_t val, size_t upto)
{
    for (size_t i = 0; i < upto; ++i)
        if (seq_cache[i] == val)
            return true;
    return false;
}

/* Absolute value for seq_t. Assumes magnitude fits in seq_t. */

seq_t a(idx_t n)
{
    /* Ensure the first term is cached. */
    if (seq_len == 0) {
        seq_cap = 16;
        seq_cache = (seq_t *)realloc(NULL, seq_cap * sizeof(seq_t));
        seq_cache[0] = 1;          /* a(1) = 1 */
        seq_len  = 1;
        seq_sum  = 1;
    }

    if (n == 1)
        return 1;

    if (n <= seq_len)          /* already computed */
        return seq_cache[n - 1];

    /* Compute terms up to n. */
    for (idx_t idx = seq_len + 1; idx <= n; ++idx) {
        seq_t prev_sum = seq_sum;
        seq_t best_a = 0;
        seq_t best_abs = -1;
        bool found = false;

        /* Iterate over positive divisors of idx. */
        for (idx_t d = 1; d * d <= idx; ++d) {
            if (idx % d != 0)
                continue;

            /* Process divisor d (positive). */
            if (d > 1) {
                seq_t cand = (seq_t)d - prev_sum;          /* S = +d */
                if (!used_in_cache(cand, seq_len)) {
                    seq_t cand_abs = abs_seq(cand);
                    if (!found || cand_abs < best_abs) {
                        best_a = cand;
                        best_abs = cand_abs;
                        found = true;
                    }
                }

                cand = -(seq_t)d - prev_sum;              /* S = -d */
                if (!used_in_cache(cand, seq_len)) {
                    seq_t cand_abs = abs_seq(cand);
                    if (!found || cand_abs < best_abs) {
                        best_a = cand;
                        best_abs = cand_abs;
                        found = true;
                    }
                }
            }

            /* Process the paired divisor e = idx / d, if distinct. */
            idx_t e = idx / d;
            if (e != d && e > 1) {
                seq_t cand = (seq_t)e - prev_sum;          /* S = +e */
                if (!used_in_cache(cand, seq_len)) {
                    seq_t cand_abs = abs_seq(cand);
                    if (!found || cand_abs < best_abs) {
                        best_a = cand;
                        best_abs = cand_abs;
                        found = true;
                    }
                }

                cand = -(seq_t)e - prev_sum;              /* S = -e */
                if (!used_in_cache(cand, seq_len)) {
                    seq_t cand_abs = abs_seq(cand);
                    if (!found || cand_abs < best_abs) {
                        best_a = cand;
                        best_abs = cand_abs;
                        found = true;
                    }
                }
            }
        }

        /* Extend cache storage if necessary. */
        if (seq_len >= seq_cap) {
            seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
            seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        }

        seq_cache[seq_len++] = best_a;
        seq_sum += best_a;
    }

    return seq_cache[n - 1];
}

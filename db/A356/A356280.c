/*
 * A356280  a(n) = Sum_{k=0..n} binomial(2*n, n-k) * p(k), where p(k) is the
 *          partition function A000041.
 * https://oeis.org/A356280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n, n-k) * p(k), where p(k) is the
 *            partition function A000041
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* ---------- compute partitions up to n ---------- */
    static seq_t *part   = NULL;
    static idx_t part_cap = 0;

    if (n >= part_cap) {
        /* Allocate or grow the array */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_part = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (!new_part) return 0;               /* out of memory, return 0 */
        if (part) {
            for (idx_t i = 0; i < part_cap; ++i)
                new_part[i] = part[i];
            free(part);
        }
        /* initialise the new entries */
        if (part_cap == 0) {          /* first time */
            new_part[0] = 1;
            part_cap = 1;
        }
        for (idx_t i = part_cap; i <= n; ++i) {
            seq_t sum_pos = 0, sum_neg = 0;
            for (idx_t k = 1;; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > i) break;
                if (k & 1) sum_pos += new_part[i - pent1];
                else       sum_neg += new_part[i - pent1];
                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > i) continue;
                if (k & 1) sum_pos += new_part[i - pent2];
                else       sum_neg += new_part[i - pent2];
            }
            new_part[i] = sum_pos - sum_neg;
        }
        part = new_part;
        part_cap = new_cap;
    }

    /* ---------- compute the sum ---------- */
    seq_t result = 0;
    seq_t cur    = binom(2 * n, n);        /* C(2n, n) */

    for (idx_t k = 0; k <= n; ++k) {
        result += cur * part[k];
        if (k < n) {
            seq_t numerator   = cur * (seq_t)(n - k);
            seq_t denominator = (seq_t)(n + k + 1);
            cur = numerator / denominator;   /* exact division */
        }
    }
    return result;
}

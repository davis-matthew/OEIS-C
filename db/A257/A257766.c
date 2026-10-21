/*
 * A257766  Numbers n such that the sum of the digits of n times the square of
 *          the sum of the digits squared of n equals n.
 * https://oeis.org/A257766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = the sorted list of integers x such that x = (sum of
 *            digits of x) * (sum of squares of digits of x)^2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   digit_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function precomputes the finite sequence once and returns
   the nth element (1‑based). If n is beyond the length of the
   sequence the function returns 0. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sums.h"

static seq_t terms[40000];
static size_t term_count = 0;
static bool initialized = false;

/* helper comparisons for qsort (seq_t is unsigned) */
static int cmp_seq_t_custom(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* maximum digit sum for 8‑digit numbers: 9*8 = 72 */
        /* maximum sum of squares for 8‑digit numbers: 81*8 = 648 */
        const int MAX_DIGIT_SUM = 72;
        const int MAX_SQR_SUM   = 648;

        for (int s1 = 1; s1 <= MAX_DIGIT_SUM; ++s1) {
            for (int s2 = 1; s2 <= MAX_SQR_SUM; ++s2) {
                seq_t s2sq = (seq_t)s2 * (seq_t)s2;
                seq_t x = (seq_t)s1 * s2sq;
                seq_t sum, sum_sq;
                digit_sums(x, &sum, &sum_sq);
                if (sum == (seq_t)s1 && sum_sq == (seq_t)s2) {
                    terms[term_count++] = x;
                }
            }
        }
        /* sort the terms */
        qsort(terms, term_count, sizeof(seq_t), cmp_seq_t_custom);
        initialized = true;
    }

    if (n == 0) return 0;                 /* offset is 1, ignore n==0 */
    if ((idx_t)(n - 1) >= term_count)     /* request beyond sequence length */
        return 0;
    return terms[n - 1];
}

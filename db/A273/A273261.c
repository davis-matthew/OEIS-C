/*
 * A273261  Irregular triangle read by rows: T(n,k) = sum of the elements of
 *          the k-th row of the difference table of the divisors of n.
 * https://oeis.org/A273261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is obtained by concatenating the rows of the irregular
 *            triangle T(r,k) where T(r,k) = sum of the elements of the k‑th
 *            row of the difference table of the divisors of r (k =
 *            0,1,…,τ(r)−1, τ(r) = number of divisors of r). Therefore
 *            a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1), a(4)=T(3,0), …
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/divisor_count_u64.h"

/* Comparator for qsort (ascending order). */
static int seq_cmp(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a > b) - (a < b);
}

/* Return the sum of the k‑th row of the difference table of the divisors
   of n (k starts at 0). */
static seq_t row_sum_divisors(idx_t n, idx_t k)
{
    /* Step 1: list all divisors of n. */
    uint64_t i;
    size_t est = 0;
    for (i = 1; i * i <= (uint64_t)n; ++i)
        if (n % i == 0) est += (i == n / i) ? 1 : 2;

    seq_t *divs = (seq_t *)malloc(est * sizeof(seq_t));
    size_t dlen = 0;
    for (i = 1; i * i <= (uint64_t)n; ++i) {
        if (n % i == 0) {
            divs[dlen++] = (seq_t)i;
            if (i != n / i)
                divs[dlen++] = (seq_t)(n / i);
        }
    }
    /* Sort divisors ascending. */
    qsort(divs, dlen, sizeof(seq_t), seq_cmp);

    /* Step 2: iterate the difference table until row k is reached. */
    seq_t sum = 0;
    for (size_t step = 0; step <= k; ++step) {
        /* Sum current row. */
        sum = 0;
        for (size_t idx = 0; idx < dlen; ++idx)
            sum += divs[idx];
        if (step == k) {
            free(divs);
            return sum;
        }

        /* Compute next row (differences). */
        seq_t *next = (seq_t *)malloc((dlen - 1) * sizeof(seq_t));
        for (size_t idx = 0; idx + 1 < dlen; ++idx)
            next[idx] = divs[idx + 1] - divs[idx];
        free(divs);
        divs = next;
        dlen--;
    }

    /* Should never reach here for valid k. */
    free(divs);
    return 0;
}

seq_t a(idx_t n)
{
    /* The sequence is obtained by concatenating rows of the triangle. */
    idx_t cum = 0;          /* cumulative number of terms processed. */
    for (idx_t r = 1; ; ++r) {
        idx_t t = divisor_count_u64(r);   /* τ(r) */
        if (cum + t >= n) {               /* the desired term is in row r */
            idx_t k = n - cum - 1;         /* column index (0‑based) */
            return row_sum_divisors(r, k);
        }
        cum += t;
    }
}

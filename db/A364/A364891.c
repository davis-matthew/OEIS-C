/*
 * A364891  Triangle read by rows: T(n,k) = (-1)^(k-1)*Sum_{j=0..k-1}
 *          (-1)^j*(p(n - j*(2*j + 1)) - p(n - (j + 1)*(2*j + 1))), where p(n)
 *          = A000041(n) is the number of partitions of n, and 1 <= k <= n.
 * https://oeis.org/A364891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = (-1)^(k-1) * sum_{j=0..k-1} (-1)^j
 *            * (p(r - j*(2*j+1)) - p(r - (j+1)*(2*j+1)))
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p(m) is the partition number A000041(m).  The triangle is read by rows. */

#include <stdbool.h>

/* The routine needs a table of partition numbers up to a modest size.  */
/* For flattened indices up to 10000 the row index `r` never exceeds 200. */
/* We compute partitions up to 1024 to be safe. */
#define PART_MAX 1024

static seq_t part[PART_MAX + 1];
static bool part_init = false;

/* Compute the integer partitions p(0..PART_MAX) using the generalized pentagonal theorem. */
static void init_partitions(void)
{
    if (part_init)
        return;

    part[0] = (seq_t)1;   /* p(0) = 1 */
    for (idx_t n = 1; n <= PART_MAX; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n)
                break;
            if (k & 1)
                sum += part[n - pent1];
            else
                sum -= part[n - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= n) {
                if (k & 1)
                    sum += part[n - pent2];
                else
                    sum -= part[n - pent2];
            }
        }
        part[n] = sum;
    }
    part_init = true;
}

/* Find the row (r) and column (k) for the flattened index idx (1‑based). */
static void row_col(idx_t idx, idx_t *r, idx_t *k)
{
    /* Estimate r via integer loop – the value of r is at most about sqrt(2*idx). */
    idx_t rr = 0;
    while ((rr * (rr + 1)) / 2 < idx)
        ++rr;
    *r = rr;
    idx_t prev_sum = ((rr - 1) * rr) / 2;
    *k = idx - prev_sum;
}

/* ------------------------------------------------------------- */
/* Public entry point                                       */
seq_t a(idx_t idx)
{
    /* Below the offset we may return 0 – the value is unspecified. */
    if (idx < 1)
        return (seq_t)0;

    init_partitions();           /* Ensure the partition table is ready. */

    idx_t r, k;
    row_col(idx, &r, &k);        /* Convert flattened index to (r,k). */

    /* Compute T(r,k). */
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j < k; ++j) {
        idx_t shift1 = r - j * (2 * j + 1);
        idx_t shift2 = r - (j + 1) * (2 * j + 1);

        seq_t p1 = (shift1 <= PART_MAX) ? part[shift1] : (seq_t)0;
        seq_t p2 = (shift2 <= PART_MAX) ? part[shift2] : (seq_t)0;

        seq_t diff = p1 - p2;
        if (j & 1)
            sum -= diff;
        else
            sum += diff;
    }

    if (k & 1)
        return sum;   /* (-1)^(k-1) = +1 when k is odd. */
    else
        return -sum;  /* (-1)^(k-1) = -1 when k is even. */
}

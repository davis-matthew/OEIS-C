/*
 * A339106  Triangle read by rows: T(n,k) = A000203(n-k+1)*A000041(k-1), n >=
 *          1, 1 <= k <= n.
 * https://oeis.org/A339106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = σ(row−k+1) * p(k−1) , where * row = smallest integer with
 *            row(row+1)/2 ≥ n * k = n − row(row−1)/2 * σ(m) = sum of divisors
 *            of m (A000203) * p(m) = partition number of m (A000041) * * The
 *            function computes the n‑th element of the triangle read by rows,
 *            * with rows indexed from 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* ---------- partition numbers up to 140 (max needed) ---------- */
static seq_t part[142] = { 0 };          /* part[0]..part[141] */
static idx_t part_max = (idx_t)-1;       /* highest p computed (-1 => none) */

/* compute p(m) for 0 <= m <= 141 */
static seq_t partition(idx_t m)
{
    if (m == 0) return 1;          /* p(0) = 1 */

    /* initialise if first use */
    if (part_max == (idx_t)-1) {
        part[0] = 1;
        part_max = 0;
    }

    /* compute up to m if necessary */
    if (m > part_max) {
        for (idx_t n = part_max + 1; n <= m; ++n) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (idx_t k = 1; ; ++k) {
                /* generalized pentagonal numbers */
                idx_t g1 = (k * (3 * k - 1)) / 2;
                if (g1 > n) break;           /* g2 will also be > n */
                seq_t val = part[n - g1];
                if (k & 1)   sum_pos += val;  /* (−1)^(k+1) = +1 when k odd */
                else         sum_neg += val;  /*   −1 when k even */

                idx_t g2 = (k * (3 * k + 1)) / 2;
                if (g2 > n) continue;
                val = part[n - g2];
                if (k & 1)   sum_pos += val;
                else         sum_neg += val;
            }
            part[n] = sum_pos - sum_neg;     /* sum_pos ≥ sum_neg */
        }
        part_max = m;
    }

    return part[m];
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* previous row sum = r(r-1)/2 */
    idx_t prev = sum - r;
    idx_t k = n - prev;              /* 1 ≤ k ≤ r */

    /* Compute σ(r−k+1) × p(k−1) */
    seq_t sigma_val = sigma_of(r - k + 1);
    seq_t part_val  = partition(k - 1);

    return sigma_val * part_val;
}

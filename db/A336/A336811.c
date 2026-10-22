/*
 * A336811  Irregular triangle read by rows T(n,k) in which the length of row
 *          n equals the partition number A000041(n-1) and every column k
 *          gives the positive integers A000027, with n >= 1 and k >= 1.
 * https://oeis.org/A336811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 265, u16 n <= 8915, u32 n <= 8915, u64 n <= 8915,
 *            u128 n <= 8915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336811: Irregular triangle read by rows.
   Formula: a(n) = 1 + (r - s)
   where r is the row that contains the n‑th flattened term,
   k = the column index inside that row,
   and s is the least row for which column k exists
   (i.e., the smallest s with p(s-1) ≥ k). */

#include <stdlib.h>

/* Partition numbers p(m), m ≥ 0, with p(0) = 1, stored as seq_t. */
static seq_t *part = NULL;
static idx_t cap_part = 0;
static idx_t size_part = 0;

/* Ensure that p(0)..p(N) are recorded in part[]. */
static void ensure_partition(idx_t N)
{
    if (N < size_part) return;          /* already available */

    /* Grow the array if necessary. */
    if (size_part + 1 > cap_part) {
        size_t newcap = cap_part ? (size_t)cap_part * 2 : 16;
        while (newcap <= (size_t)N + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(part, newcap * sizeof(seq_t));
        if (!tmp) abort();
        part = tmp;
        cap_part = (idx_t)newcap;
    }

    /* Compute missing partition numbers. */
    for (; size_part <= N; ++size_part) {
        idx_t m = size_part;
        seq_t sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            if (k & 1) sum += part[m - g1];
            else       sum -= part[m - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m) continue;
            if (k & 1) sum += part[m - g2];
            else       sum -= part[m - g2];
        }
        part[size_part] = sum;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */

    /* Initialise partition array if necessary. */
    if (!part) {
        cap_part   = 16;
        size_part  = 1;                  /* p(0) will be stored */
        part        = (seq_t *)malloc(cap_part * sizeof(seq_t));
        if (!part) abort();
        part[0] = (seq_t)1;              /* p(0) = 1 */
    }

    /* Find the row r that contains the n‑th term. */
    seq_t target = (seq_t)n;
    seq_t cumulative = (seq_t)0;          /* total length of rows before r */
    idx_t r = 1;
    for (;;) {
        ensure_partition(r - 1);
        seq_t len = part[r - 1];
        if (cumulative + len < target) {
            cumulative += len;
            ++r;
        } else {
            break;                       /* row r contains the term */
        }
    }

    /* Column position k (1‑based) within row r. */
    seq_t k_seq = target - cumulative;    /* 1 ≤ k ≤ length of row r */
    idx_t k = (idx_t)k_seq;               /* safe because k ≤ p(r-1) ≤ n */

    /* Find the first row s where column k exists. */
    idx_t s = 1;
    for (;;) {
        ensure_partition(s - 1);
        if (part[s - 1] >= k_seq) break;
        ++s;
    }

    /* Return the value in the triangle: 1 + (r - s). */
    return (seq_t)(1 + (r - s));
}

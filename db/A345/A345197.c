/*
 * A345197  Concatenation of square matrices A(n), each read by rows, where
 *          A(n)(k,i) is the number of compositions of n of length k with
 *          alternating sum i, where 1 <= k <= n, and i ranges from -n + 2 to
 *          n in steps of 2.
 * https://oeis.org/A345197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345197:
   a(n) is the n‑th term of the concatenation of the square matrices A(m),
   read row‑wise.  A(m)(k,i) equals the number of compositions of m of length
   k with alternating sum i (=y₁−y₂+⋯).  The first term a(0) is the single
   entry of A(1). */

#include "../utils/binom.h"
#include <stdint.h>

static seq_t count_entry(idx_t n, idx_t k, int64_t i)
{
    idx_t o = (k + 1) / 2;     /* number of odd positions */
    idx_t e = k / 2;           /* number of even positions */

    int64_t U = ((int64_t)n + i) / 2;
    int64_t V = ((int64_t)n - i) / 2;

    if (U < (int64_t)o) return (seq_t)0;
    if (e > 0 && V < (int64_t)e) return (seq_t)0;

    if (e == 0) {                /* no even parts */
        if (V != 0) return (seq_t)0;
        return binom((idx_t)(U - 1), (idx_t)(o - 1));
    } else {
        seq_t a1 = binom((idx_t)(U - 1), (idx_t)(o - 1));
        seq_t a2 = binom((idx_t)(V - 1), (idx_t)(e - 1));
        return a1 * a2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* first entry of A(1) */

    idx_t pos = n;                         /* current index in concatenated list */
    idx_t m   = 1;
    idx_t sum = 0;

    /* find the matrix containing the desired term */
    while (sum + m * m <= pos) {
        sum += m * m;
        m++;
    }

    idx_t offset = pos - sum;              /* 0‑based offset inside A(m) */
    idx_t k      = offset / m + 1;         /* row number (length k) */
    idx_t col    = offset % m;             /* column index for i */
    int64_t i    = -((int64_t)m) + 2 + 2 * (int64_t)col;

    return count_entry((idx_t)m, k, i);
}

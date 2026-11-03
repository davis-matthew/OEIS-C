/*
 * A318772  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1,k) + 3 *
 *          T(n-4,k-1) for k = 0..floor(n/4); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A318772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 101, u32 n <= 319, u64 n <= 1120,
 *            u128 n <= 1890, big any n
 * Validated: standard, pass, 88 terms through n = 1832 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: a(n) is the n‑th element in rows of
 * T(n,k) defined by T(0,0)=1, T(n,k)=T(n-1,k)+3*T(n-4,k-1) for
 * 0 ≤ k ≤ ⌊n/4⌋.  Equivalently,
 * T(n,k)=3^k * (n‑3k)! / ((n‑4k)! k!).
 * The sequence a(n) enumerates the triangle row by row
 * (k runs from 0 upward for each n).
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the number of terms in rows 0 … r (inclusive).
 * Returns a 128‑bit value to avoid overflow. */
static inline unsigned __int128 total_terms_up_to_row(idx_t r)
{
    idx_t q = r / 4;          /* floor(r/4)       */
    idx_t rem = r & 3;        /* r % 4            */

    /* sum_{i=0}^r floor(i/4) = q*(2*q+rem-1)   */
    unsigned __int128 sum_floor =
        (unsigned __int128)q * (unsigned __int128)(2 * (unsigned __int128)q + rem - 1);

    return (unsigned __int128)(r + 1) + sum_floor;
}

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of terms
     * exceeds n.  Use binary search over r.  For any n we know
     * that total_terms_up_to_row(r) grows ~ r²/8, so r ≤ n for all n>0. */
    idx_t low = 0;
    idx_t high = n;             /* high is at least n (or 0) */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (total_terms_up_to_row(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;            /* the row containing the n‑th term */

    /* Compute start index of this row */
    idx_t prev_total = (row == 0) ? 0 : (idx_t)total_terms_up_to_row(row - 1);
    idx_t k = n - prev_total;   /* position within the row – this is the k-value */

    /* T(row,k) = 3^k * C(row-3k, k)   (C = binomial) */
    seq_t binom = 1;            /* compute C(row-3k, k) multiplicatively */
    idx_t n_param = row - 3 * k; /* n-3k in the binomial */
    for (idx_t i = 1; i <= k; ++i) {
        binom = binom * ((seq_t)n_param - (seq_t)i + 1);
        binom = binom / (seq_t)i;
    }

    seq_t pow3 = 1;             /* compute 3^k */
    for (idx_t i = 1; i <= k; ++i)
        pow3 *= 3;

    return binom * pow3;
}

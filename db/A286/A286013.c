/*
 * A286013  Irregular triangle read by rows: T(n,k), n>=1, k>=1, in which
 *          column k lists the positive integers starting with k, interleaved
 *          with k-1 zeros, and the first element of column k is in row
 *          k(k+1)/2.
 * https://oeis.org/A286013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 9816, u32 n <= 9816, u64 n <= 9816,
 *            u128 n <= 9816, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A286013 – Irregular triangle read by rows.  
 *  The sequence is obtained by reading the triangle T(n,k) defined in the
 *  OEIS entry:
 *
 *     T(n,k) = k + (n - k(k+1)/2)/k   if  n ≥ k(k+1)/2  and
 *                              (n - k(k+1)/2) mod k = 0,
 *     T(n,k) = 0                      otherwise.
 *
 *  For a given linear index n (n ≥ 1) the algorithm locates the row r
 *  and the column k such that the cumulative number of entries up to
 *  row r-1 is < n ≤ cumulative up to row r.  The column k is
 *  (n - S(r-1) - 1) + 1, where S(r) = Σ_{i=1..r} floor((√(8i+1)-1)/2).
 *
 *  This implementation uses only integer arithmetic; it relies on the
 *  helper function floor_sqrt from the utilities.  It works for any
 *  integer type seq_t/idx_t provided by the build.
 */

#include "../utils/floor_sqrt.h"

/* triangular number k(k+1)/2 as seq_t */
static seq_t triangular(idx_t k)
{
    return (seq_t)k * ((seq_t)k + (seq_t)1) / (seq_t)2;
}

/* a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n < 1 is not used by OEIS */

    idx_t r = 1;          /* current row */
    seq_t sum = 0;        /* cumulative number of entries up to row r-1 */

    while (1) {
        /* Kmax(r) = max k with triangular(k) <= r */
        seq_t s    = (seq_t)r;
        seq_t sq   = s * (seq_t)8 + (seq_t)1;
        seq_t ssq  = floor_sqrt(sq);                 /* sqrt(8r+1) */
        idx_t kmax = (idx_t)((ssq - (seq_t)1) / (seq_t)2);
        seq_t kmax_seq = (seq_t)kmax;

        if (sum + kmax_seq >= (seq_t)n) {
            /* the desired entry lies in row r */
            seq_t idx_in_row = (seq_t)n - sum - (seq_t)1;   /* 0‑based */
            idx_t k = (idx_t)idx_in_row + (idx_t)1;        /* 1‑based */

            seq_t Tk = triangular(k);
            seq_t r_seq = (seq_t)r;

            if (r_seq >= Tk) {
                seq_t delta = r_seq - Tk;                 /* >=0 */
                if (delta % (seq_t)k == 0)
                    return (seq_t)k + delta / (seq_t)k;
            }
            return 0;      /* column defined but not active -> 0 */
        }

        /* move to next row */
        sum += kmax_seq;
        r++;
    }
}

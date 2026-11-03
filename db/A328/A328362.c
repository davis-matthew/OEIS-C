/*
 * A328362  Triangle read by rows: T(n,k) is the sum of all parts k in all
 *          partitions of n into consecutive parts, (1 <= k <= n).
 * https://oeis.org/A328362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A328362 – a(n) is the nth entry of the triangle T(n,k) where
   T(n,k) = k * (#of consecutive-interval partitions of n that contain k).
   The function enumerates all partitions of n into consecutive integers
   (intervals) and counts those that contain k.  The triangle is read
   by rows, so for input n the function finds the row r and column k
   such that r(r–1)/2 < n ≤ r(r+1)/2 and returns T(r,k). */
#include <stdint.h>

static seq_t T_consecutive_parts(idx_t n, idx_t k)
{
    /* Count intervals [a,b] such that sum a..b = n and a ≤ k ≤ b. */
    idx_t two_n = n * 2;
    idx_t count = 0;

    for (idx_t r = 1; r * r <= two_n; ++r) {
        if (two_n % r != 0) continue;

        /* ---------- process interval of length r ---------- */
        {
            idx_t len = r;
            idx_t q   = two_n / len;                 /* q = 2n / len   */
            long long tmp = (long long)q - (long long)len + 1; /* tmp = (2n/len) - len + 1 */
            if (tmp > 0 && (tmp & 1LL) == 0) {
                seq_t first = (seq_t)(tmp / 2);
                if (first >= 1) {
                    seq_t last = first + len - 1;
                    if ((seq_t)k >= first && (seq_t)k <= last) ++count;
                }
            }
        }

        /* ---------- process complementary divisor d = two_n/r ----------
           (if distinct from r) ---------- */
        idx_t d = two_n / r;
        if (d != r) {
            idx_t len = d;
            idx_t q   = two_n / len;                 /* q = 2n / len   */
            long long tmp = (long long)q - (long long)len + 1; /* tmp = (2n/len) - len + 1 */
            if (tmp > 0 && (tmp & 1LL) == 0) {
                seq_t first = (seq_t)(tmp / 2);
                if (first >= 1) {
                    seq_t last = first + len - 1;
                    if ((seq_t)k >= first && (seq_t)k <= last) ++count;
                }
            }
        }
    }

    return (seq_t)k * (seq_t)count;
}

static idx_t find_row(idx_t n)
{
    idx_t r = 1;
    idx_t cum = 1;                 /* cumulative entries up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, below 1 not needed */

    idx_t row = find_row(n);
    idx_t prev = row * (row - 1) / 2;   /* entries before this row */
    idx_t col = n - prev;               /* 1-based column in the row */

    return T_consecutive_parts(row, col);
}

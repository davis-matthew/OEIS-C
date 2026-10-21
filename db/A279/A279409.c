/*
 * A279409  Triangle read by rows: T(n,m) (n>=m>=1) = maximum number of
 *          nonattacking kings on an n X m toroidal board.
 * https://oeis.org/A279409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,m) = max number of non‑attacking kings on an n×m toroidal
 *            board. Sequence a(k) is the triangle read by rows: a(1)=T(1,1),
 *            a(2)=T(2,1), a(3)=T(2,2), ... This implementation finds the row
 *            r of index n and the column m, then evaluates the explicit
 *            formula described in the OEIS entry
 * Range:     u8 n <= 526, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute maximal number of kings on an n×m toroidal board. */
static seq_t T_row_col(idx_t n, idx_t m)
{
    /* Special case 1×1 board. */
    if (n == 1 && m == 1) return 1;

    /* For any board with one dimension equal to 1 (and >1), the answer is floor(n/2). */
    if (m == 1) {
        return (seq_t)(n / 2);
    }

    /* General case: floor(min(m*floor(n/2), n*floor(m/2))/2) */
    seq_t half_n = (seq_t)(n / 2);
    seq_t half_m = (seq_t)(m / 2);
    seq_t a = (seq_t)m * half_n;
    seq_t b = (seq_t)n * half_m;
    seq_t min_ab = a < b ? a : b;
    return min_ab / 2;
}

seq_t a(idx_t N)
{
    /* Find the row r such that r*(r+1)/2 >= N.
       Use binary search with 128‑bit arithmetic to avoid overflow. */
    if (N == 0) return 0;          /* undefined for N<1, but safe */
    idx_t low = 1, high = 1;

    /* Exponentially increase high until high*(high+1)/2 >= N */
    while ((unsigned __int128)high * (high + 1) / 2 < (unsigned __int128)N) {
        high <<= 1;
        if (high == 0) {          /* overflow protection: high reached max value */
            high = UINT64_MAX;
            break;
        }
    }

    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= (unsigned __int128)N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low; /* row number */

    /* Cumulative count up to previous row */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t offset = N - (idx_t)prev_tri - 1;  /* zero‑based offset within the row */
    idx_t m = offset + 1;                    /* column index */

    return T_row_col(r, m);
}

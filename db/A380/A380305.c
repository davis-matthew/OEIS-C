/*
 * A380305  Triangle read by rows: T(n,k) = (n - floor((n - k)/k)) mod k, for
 *          0 < k <= n.
 * https://oeis.org/A380305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r - floor((r - c)/c)) mod c, where r and c are the row
 *            and column indices in the triangle T(r,c) = (r - floor((r -
 *            c)/c)) mod c, and a is read by rows
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper to find the row number r such that r(r+1)/2 >= n
   using a binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t
row_of_index(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* find the row r and column c corresponding to the index n */
    idx_t r = row_of_index(n);
    /* total entries before row r */
    __uint128_t prev = (__uint128_t)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev;  /* 1‑based column */

    /* compute T(r,c) = (r - floor((r - c)/c)) mod c */
    idx_t quot = (r - c) / c;          /* floor((r-c)/c) */
    idx_t val  = (r - quot) % c;       /* modulo c */

    return (seq_t)val;
}

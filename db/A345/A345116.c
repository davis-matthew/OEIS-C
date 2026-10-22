/*
 * A345116  Irregular triangle T(n,k) read by rows in which row n has length
 *          the n-th triangular number A000217(n) and every column k lists the
 *          positive integers A000027, n >= 1, k >= 1.
 * https://oeis.org/A345116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The sequence a(n) is the n-th element of the irregular triangle
   T(n,k).  Row n has length T(n) = n(n+1)/2 and contains the values
   n repeated once, n-1 repeated twice, …, 1 repeated n times, in
   non‑increasing order.  The triangle is read row‑wise (by rows),
   and a(n) is the value at the global index n (offset 1).
*/

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/* Integer square root of a 64‑bit unsigned integer.  Returns floor(sqrt(x))  */
/* -------------------------------------------------------------------------- */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t res = 0;
    idx_t bit = (idx_t)1 << 62;           /* highest power of four <= 2^63   */
    while (bit > x) bit >>= 2;           /* find suitable starting bit      */
    for (; bit != 0; bit >>= 2)
    {
        if (x >= res + bit)
        {
            x   -= res + bit;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
    }
    return res;
}

/* -------------------------------------------------------------------------- */
/* Total number of terms up to row r (inclusive).  Formula: r(r+1)(r+2)/6    */
/* -------------------------------------------------------------------------- */
static inline idx_t cumulative_terms(idx_t r)
{
    /* Use 128‑bit intermediate to avoid overflow of 64‑bit product   */
    __int128 prod = (__int128)r * (r + 1) * (r + 2);
    prod /= 6;
    if (prod > (__int128)UINT64_MAX)            /* keep value within bounds  */
        return UINT64_MAX;
    return (idx_t)prod;
}

/* -------------------------------------------------------------------------- */
/* Find 1‑based row index r such that cumulative_terms(r‑1) < n ≤ cumulative_terms(r)  */
/* -------------------------------------------------------------------------- */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Exponential search for an upper bound */
    while (cumulative_terms(high) < n)
    {
        if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }

    /* Binary search within [low, high] */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (cumulative_terms(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* -------------------------------------------------------------------------- */
/* Main sequence function                                                     */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset 1: undefined for n == 0 */

    /* 1. Identify the row that contains the global index n */
    idx_t row = find_row(n);

    /* 2. Index of the position within that row (1‑based) */
    idx_t prev_total = cumulative_terms(row - 1);
    idx_t pos_in_row = n - prev_total;   /* 1 ≤ pos_in_row ≤ T(row) */

    /* 3. Determine which group within the row (values n, n‑1, …, 1)  */
    /* Groups run 1,2,…,row times.  Find smallest t with t(t+1)/2 ≥ pos_in_row. */
    idx_t disc = 8 * pos_in_row + 1;          /* 64‑bit, fits for all practical n  */
    idx_t sqrt_disc = isqrt_u64(disc);        /* floor(sqrt(8*pos+1))              */
    idx_t t = (sqrt_disc - 1) / 2;            /* initial guess                     */
    if ((t * (t + 1)) / 2 < pos_in_row) t++; /* adjust if needed                  */

    /* 4. Compute the value at this position */
    idx_t value = row - t + 1;                /* range 1 … row                     */

    return (seq_t)value;
}

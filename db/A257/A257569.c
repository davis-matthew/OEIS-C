/*
 * A257569  Triangular array read by rows: T(h,k) = number of steps from (h,k)
 *          to (0,0), where one step is (x,y) -> (x-1, y) if x is odd or (x,y)
 *          -> (y, x/2) if x is even.
 * https://oeis.org/A257569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute terms of OEIS sequence A257569.
 * The sequence is the triangular array T(h,k) read by rows where
 * T(h,k) is the number of steps from (h,k) to (0,0) using the
 * deterministic rule:
 *   if x is odd  -> (x-1, y)
 *   if x is even -> (y, x/2)
 * Rows correspond to antidiagonals: each row h contains
 * (h,0),(h-1,1),...,(0,h).
 * For a given n (1‑based) we find the unique h and k such that
 * n is the (k+1)‑st element of the h‑th row and then evaluate
 * T(h-k, k).
 * -------------------------------------------------------------
 *  No external libraries or tables are used; the algorithm runs
 *  in O(log n + steps(h-k,k)) time, fast enough for n up to at
 *  least 10 000.
 */
#include <stdint.h>

/* Forward declaration of helper function */
static seq_t compute_T(idx_t h, idx_t k);

/* Main function required by the build */
seq_t a(idx_t n)
{
    /* Find the smallest h such that (h+1)(h+2)/2 >= n */
    idx_t low = 0, high = 1;
    /* Grow high until it satisfies the condition */
    while ((unsigned __int128)(high + 1) * (high + 2) / 2 < n)
        high <<= 1;
    /* Binary search */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)(mid + 1) * (mid + 2) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t h = low;

    /* Start index of row h (1‑based) */
    unsigned __int128 tmp_start =
        1 + (unsigned __int128)h * (h + 1) / 2;
    idx_t start = (idx_t)tmp_start;

    /* k is the offset within the row (0‑based) */
    idx_t k = n - start;
    /* Corresponding coordinates in the array are (h - k, k) */
    idx_t x = h - k;
    idx_t y = k;

    return compute_T(x, y);
}

/* Count steps from (h,k) to (0,0) following the deterministic rule. */
static seq_t compute_T(idx_t h, idx_t k)
{
    idx_t x = h;
    idx_t y = k;
    seq_t steps = 0;
    while (x || y) {
        if (x & 1) {
            /* x is odd: (x-1, y) */
            x -= 1;
        } else {
            /* x is even: (y, x/2) */
            idx_t tmp = x >> 1;   /* x/2 */
            x = y;
            y = tmp;
        }
        steps += 1;
    }
    return steps;
}

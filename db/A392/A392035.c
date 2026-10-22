/*
 * A392035  Irregular table read by rows: the n-th row contains, in weakly
 *          decreasing order, the positive integers whose squares sum to n as
 *          obtained by the greedy algorithm.
 * https://oeis.org/A392035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  a(n) = the n‑th term of the flattened greedy square‑sum table.
 *  The n‑th row contains the numbers obtained by repeatedly subtracting
 *  the largest possible square from the remaining sum.  This function
 *  returns the n‑th entry read row by row, starting at a(1)=1.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Integer square root for idx_t (uint64_t) using binary search. */
static idx_t
isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t lo = 1, hi = n >> 1U;  /* for n >= 4, sqrt(n) <= n/2 */
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2U;
        /* use 128‑bit to avoid overflow of mid*mid */
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            if (mid + 1 > n / (mid + 1))
                return mid;                /* next would overflow */
            lo = mid + 1;
        } else {
            if (mid == 0)  /* guard against infinite loop for n==0 */
                return 0;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* Public function */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; a(0) is undefined but we return 0. */
    if (n == 0)
        return 0;

    idx_t cumulative = 0;  /* number of elements produced so far */

    for (idx_t row = 1;; ++row) {
        idx_t rem = row;
        while (rem > 0) {
            idx_t k = isqrt_u64(rem);
            rem -= k * k;
            ++cumulative;
            if (cumulative == n)
                return (seq_t)k;      /* cast to seq_t; values are small */
        }
    }
}

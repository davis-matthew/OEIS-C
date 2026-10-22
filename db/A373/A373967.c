/*
 * A373967  Triangle read by rows: T(n,k) = (-1)^n * n! + (-1)^(k+1) * k! for
 *          n >= 2 and 1 <= k <= n-1.
 * https://oeis.org/A373967
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = (-1)^r * r! + (-1)^(k+1) * k! , where r and k are the row
 *            and column indices in the triangle T(r,k) = (-1)^r * r! +
 *            (-1)^(k+1) * k!, read by rows. The mapping from sequence index n
 *            (n ≥ 2) to (r,k) is: let s = n – 1. Find the smallest r ≥ 2 such
 *            that (r–1)*r/2 ≥ s. Then let k = s – (r–2)*(...
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t find_row(idx_t s) {
    /* Find smallest r ≥ 2 with (r-1)*r/2 ≥ s using binary search. */
    idx_t low = 2, high = 2;
    while ( ((__int128)(high-1)*high/2) < s ) {
        /* double high; guard against overflow (unlikely for the intended range) */
        if (high > ((idx_t)-1) >> 1) { high = (idx_t)-1; break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ( ((__int128)(mid-1)*mid/2) >= s )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

static seq_t factorial(idx_t m) {
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        res *= (seq_t)i;
    return res;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;            /* value for indices below the offset is irrelevant */
    idx_t s   = n - 1;               /* ordinal number of the term (starting at 1)    */
    idx_t r   = find_row(s);         /* row number (>=2)                            */
    idx_t prev = ( (__int128)(r-2)*(r-1)/2 ); /* entries before row r             */
    idx_t k   = s - prev;              /* column number (1 ≤ k ≤ r-1)                 */

    seq_t fact_r = factorial(r);
    seq_t fact_k = factorial(k);

    int sign_r  = ((r & 1u) == 0u) ? 1 : -1;        /* (-1)^r */
    int sign_k1 = (((k + 1) & 1u) == 0u) ? 1 : -1; /* (-1)^(k+1) */

    seq_t value = fact_r * (seq_t)sign_r + fact_k * (seq_t)sign_k1;
    return value;
}

/*
 * A284131  Hosoya triangle of Morgan Voyce type, read by rows.
 * https://oeis.org/A284131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the Hosoya triangle of Morgan Voyce
 *            type. * It is read by rows with the rule * T(n,k) = L(2*k) *
 *            L(2*(n-k+1)) * where L(m) denotes the m‑th Lucas number. The
 *            offset for this sequence is 1
 * Range:     u8 n <= 10, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Return the m‑th Lucas number.  L(0)=2, L(1)=1, L(m)=L(m-1)+L(m-2).  */
static seq_t lucas_n(idx_t m)
{
    /* L(m) = F(m-1) + F(m+1).  Using the pre‑written fib function. */
    return fib(m - 1) + fib(m + 1);
}

/* Compute the triangular index r such that r(r+1)/2 >= n
 * and the column index k (1‑based).  Then return T(r,k). */
seq_t a(idx_t n)
{
    /* Find row r using binary search on triangular numbers. */
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 < n)
        high <<= 1;          /* double until high covers n */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* row number (1‑based) */

    idx_t prev_terms = (r - 1) * r / 2;  /* terms before row r */
    idx_t k = n - prev_terms;           /* column number (1‑based) */

    /* Compute the two Lucas factors and multiply them. */
    seq_t lucas_k   = lucas_n(2 * k);
    seq_t lucas_rk  = lucas_n(2 * (r - k + 1));
    return lucas_k * lucas_rk;
}

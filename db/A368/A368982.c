/*
 * A368982  Triangle read by rows: T(n, k) = binomial(n, k - 1) * (k - 1)^(k -
 *          1) * (n - k) * (n - k + 1)^(n - k) / 2.
 * https://oeis.org/A368982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n, k) = binomial(n, k-1) * (k-1)^(k-1) * (n-k) * (n-k+1)^(n-k)
 *            / 2 a(n) is the triangle read by rows using the above formula
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Find the row index r such that
   C(r-1) <= n < C(r), where C(x) = (x+1)(x+2)/2.  */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;          /* r never exceeds n */
    while (low < high)
    {
        idx_t mid = (low + high) / 2;
        /* cumulative count up to row mid : C(mid) */
        idx_t cnt = (mid + 1) * (mid + 2) / 2;
        if (cnt > n)
            high = mid;               /* mid is too high, shrink high */
        else
            low = mid + 1;            /* mid is too low, shift low up */
    }
    return low;                       /* minimal r with C(r) > n */
}

seq_t a(idx_t n)
{
    /* Handle the trivial first term */
    if (n == 0)
        return (seq_t)0;

    /* Determine the row r and column col (0‑based) of the n‑th entry */
    idx_t r = find_row(n);
    idx_t base = r * (r + 1) / 2;      /* cumulative count before row r */
    idx_t col = n - base;              /* column within the row */

    if (col == 0)                     /* leftmost entry is always 0 */
        return (seq_t)0;

    /* Now compute the triangle value T(r, col) */
    seq_t bin      = binomial(r, col - 1);
    seq_t pow_left = big_pow((seq_t)(col - 1), (seq_t)(col - 1));
    idx_t diff     = r - col;
    seq_t pow_right = big_pow((seq_t)(diff + 1), (seq_t)diff);

    seq_t result = bin;
    result *= pow_left;
    result *= (seq_t)diff;
    result *= pow_right;
    result /= (seq_t)2;

    return result;
}

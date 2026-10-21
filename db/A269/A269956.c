/*
 * A269956  Triangle read by rows, T(n,k) = binomial(3*n,n+k) for n>=0 and
 *          0<=k<=n.
 * https://oeis.org/A269956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 80, u64 n <= 281,
 *            u128 n <= 1009, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A269956 – triangle read by rows.
   Triangle entry: T(n,k)=binomial(3*n, n+k) for n≥0, 0≤k≤n.
   The sequence a(n) is the flattening of this triangle row‑wise.

   For a given index n we determine its row r and column c
   (n = r(r+1)/2 + c, 0 ≤ c ≤ r) and return
          a(n) = binomial(3*r, r+c).
*/

#include <stdint.h>
#include "../utils/gcd.h"          /* provides seq_t gcd(seq_t, seq_t) */

/* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static idx_t _row_of(idx_t n)
{
    idx_t r   = 0;
    idx_t cum = 0;
    while (1) {
        idx_t next = cum + r + 1;     /* number of elements up to next row */
        if (next > n)
            break;
        cum = next;
        ++r;
    }
    return r;
}

/* Compute binomial coefficient C(n,k) (0 ≤ k ≤ n) using only seq_t
   arithmetic and the provided gcd function.  This keeps intermediate
   values small and works for arbitrary‑precision seq_t. */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;                       /* take the smaller side */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator   = (seq_t)(n - k + i);
        seq_t denominator = (seq_t)i;

        /* Reduce numerator and denominator by their gcd. */
        seq_t g2 = gcd(numerator, denominator);
        numerator   /= g2;
        denominator /= g2;

        /* Reduce remaining denominator with the running result. */
        seq_t g1 = gcd(result, denominator);
        result   /= g1;
        denominator /= g1;

        /* denominator should now be 1.  Multiply into result. */
        result = result * numerator;
        /* In the unlikely event denominator>1, divide it. */
        if (denominator != 1)
            result /= denominator;
    }
    return result;
}

/* Sequence entry a(n).  External linkage. */
seq_t a(idx_t n)
{
    idx_t row = _row_of(n);
    idx_t cum = row * (row + 1) / 2;      /* elements before this row */
    idx_t col = n - cum;                  /* column within the row */

    /* Compute T(row, col) = binomial(3*row, row+col). */
    return _binom(3 * row, row + col);
}

/*
 * A255168  Rectangular array A read by upward antidiagonals in which row n is
 *          the set of positive integers that are congruent to {(1 + 2^n*(3 +
 *          2*(-1)^n))/3, 2^(n + 1), (1 + 2^n*(15 + 2*(-1)^n))/3} (mod
 *          2^(n+2)).
 * https://oeis.org/A255168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry obtained by reading the rectangular array A
 *            upward antidiagonally. The array A is defined by A(n,1) = (1 +
 *            2^n * (3 + 2*(-1)^n)) / 3 A(n,2) = 2^(n+1) A(n,3) = A(n,1) +
 *            2^(n+1) For k > 3: A(n,k) = A(n,k-3) + 2^(n+2) The function
 *            below computes a(n) for any n ≥ 1. It uses the supp...
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----- helpers -------------------------------------------------------- */

#include "../utils/big_pow.h"

/* Compute 2^exp (exp is an idx_t). */
static seq_t pow2exp(idx_t exp)
{
    return big_pow((seq_t)2, (seq_t)exp);
}

/* Return the value of the array entry A(row, col). */
static seq_t A_rowcol(idx_t row, idx_t col)
{
    /* Residue in column 1. */
    seq_t r1;
    if ((row & (idx_t)1) == (idx_t)0) {
        /* even row: (1 + 2^row * 5) / 3 */
        r1 = (pow2exp(row) * (seq_t)5 + (seq_t)1) / (seq_t)3;
    } else {
        /* odd row: (2^row + 1) / 3 */
        r1 = (pow2exp(row) + (seq_t)1) / (seq_t)3;
    }

    /* Column 2 is simply 2^(row+1). */
    seq_t r2 = pow2exp(row + (idx_t)1);

    /* The three residue classes used for columns 1, 2, 3. */
    seq_t residues[3];
    residues[0] = r1;
    residues[1] = r2;
    residues[2] = r1 + r2;

    /* Modulus for shifting each full 3‑column block: 2^(row+2). */
    seq_t mod = pow2exp(row + (idx_t)2);

    /* Determine which residue applies to the requested column. */
    idx_t t = (col - (idx_t)1) % (idx_t)3;     /* 0, 1, or 2 */
    idx_t q = (col - (idx_t)1) / (idx_t)3;     /* number of full blocks */

    return residues[t] + mod * (seq_t)q;
}

/* ----- public function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;    /* undefined for n ≤ 0 */

    /* Find the smallest diagonal k such that k*(k+1)/2 ≥ n. */
    idx_t low = (idx_t)1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> (idx_t)1);
        __uint128_t val = (__uint128_t)mid * (mid + (idx_t)1) / (idx_t)2;
        if (val >= n)
            high = mid;
        else
            low = mid + (idx_t)1;
    }
    idx_t k = low;

    /* Number of elements before diagonal k. */
    idx_t prev = (k - (idx_t)1) * k / (idx_t)2;

    /* Position of n on diagonal k (1‑based). */
    idx_t p = n - prev;

    /* Convert to row/column coordinates. */
    idx_t row = k - p + (idx_t)1;
    idx_t col = p;

    return A_rowcol(row, col);
}

/*
 * A351491  Irregular triangle read by rows: T(n,k) is the minimum number of
 *          alphabetic symbols in a regular expression for the k
 *          lexicographically first palindromes of length 2*n over a ternary
 *          alphabet, n >= 0, 1 <= k <= 3^n.
 * https://oeis.org/A351491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Compute OEIS sequence A351491 (flattened irregular triangle).
 *  For row r (n) and column k (1‑indexed),
 *  T(r,k) = 2*r + 3*k - 3 - SumDigitsBase3(k-1),
 *  where SumDigitsBase3(m) is the sum of the base‑3 digits of m.
 *  This function maps the global index n to (r,k) and evaluates the formula.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r and column k such that the index n falls into row r. */
    idx_t r = 0;
    unsigned __int128 cum   = 0;   /* cumulative terms processed */
    unsigned __int128 power = 1;   /* 3^r for current row */

    while (1) {
        cum += power;           /* add terms of current row */
        if (cum > n) {          /* index falls into this row */
            break;
        }
        power *= 3;             /* advance to next row */
        r++;
    }

    /* Offset of the first term of this row in the flattened sequence */
    idx_t offset = (idx_t)(cum - power);

    /* Column k, 1‑indexed (k = index within the row + 1) */
    idx_t k = n - offset + 1;

    /* Sum of base‑3 digits of (k-1) */
    seq_t sum = 0;
    idx_t tmp = k - 1;
    while (tmp > 0) {
        sum += tmp % 3;
        tmp /= 3;
    }

    /* Compute T(r,k) = 2*r + 3*k - 3 - sum */
    seq_t term = (seq_t)2 * r + (seq_t)3 * k - (seq_t)3 - sum;
    return term;
}

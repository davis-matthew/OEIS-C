/*
 * A395426  Triangle read by rows where T(n,k) is the number of distinct n X k
 *          (n >= k) Braille patterns up to translations.
 * https://oeis.org/A395426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) = T(r,k) where * T(r,k) = 2^(r*k) - 2^(r*(k-1)) -
 *            2^((r-1)*k) + 2^((r-1)*(k-1)) + 1, * with r the smallest integer
 *            such that r*(r+1)/2 >= n * and k = n - r*(r-1)/2 (1‑based)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find row r and column k for the n‑th element of the triangle. */
    idx_t r = 1;
    idx_t cum = 1;                     /* cumulative count up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev = cum - r;              /* count before this row */
    idx_t k = n - prev;                /* 1‑based column index */

    /* Compute the exponents needed for the formula. */
    seq_t e1 = (seq_t)r * (seq_t)k;
    seq_t e2 = (seq_t)r * (seq_t)(k - 1);
    seq_t e3 = (seq_t)(r - 1) * (seq_t)k;
    seq_t e4 = (seq_t)(r - 1) * (seq_t)(k - 1);

    /* Power of two values (big_pow handles arbitrary‑precision if needed). */
    seq_t t1 = big_pow((seq_t)2, e1);
    seq_t t2 = big_pow((seq_t)2, e2);
    seq_t t3 = big_pow((seq_t)2, e3);
    seq_t t4 = big_pow((seq_t)2, e4);

    /* Final computation according to the formula. */
    seq_t result = t1 - t2;
    result -= t3;
    result += t4;
    result += 1;

    return result;
}

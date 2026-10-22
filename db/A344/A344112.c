/*
 * A344112  Triangle read by rows: T(n,k) is the number of relations from an
 *          n-element set to a k-element set that are not functions.
 * https://oeis.org/A344112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(n,k) = 2^(n*k) - k^n, a(n) reads the triangle row‑wise
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 20, u64 n <= 43, u128 n <= 76,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

/* Return the n-th term of the OEIS sequence A344112. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        r++;
    }

    /* Column k: offset within the row. */
    idx_t k = n - r * (r - 1) / 2;

    /* Compute 2^(r*k) and k^r. */
    seq_t pow2_val = pow2(r * k);
    seq_t k_pow_r  = big_pow((seq_t)k, (seq_t)r);

    return pow2_val - k_pow_r;
}

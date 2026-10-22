/*
 * A378145  Riordan triangle (1 + x * C(x), x * C(x)), where C(x) is g.f. of
 *          A000108.
 * https://oeis.org/A378145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the row‑major read of the Riordan triangle T(n,k). For a
 *            given index n, find the row r such that r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 then k = n - r*(r+1)/2 and output T(r,k) where
 *            T(r,k) = binomial(2r-k, r) * (r*(3k+1) - 2k(k+1)) /
 *            ((2r-k)(2r-k-1)) (with the convention T(r,r) = 1)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Euclidean gcd for the integer type seq_t */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute T(n,k) as described above.  Assumes 0 ≤ k ≤ n. */
static seq_t T(idx_t n, idx_t k)
{
    if (k == n)
        return (seq_t)1;

    /*  binomial(2n-k, n) */
    seq_t bin = binomial(2*n - k, n);

    /*  Numerator: n*(3k+1) - 2*k*(k+1)  */
    seq_t num = (seq_t)((__int128)n * (3*k + 1)
                       - (__int128)2 * k * (k + 1));

    /*  Denominator factors */
    seq_t den1 = (seq_t)(2*n - k);
    seq_t den2 = den1 - (seq_t)1;

    /* Reduce binomial with the denominators */
    seq_t g = seq_gcd(bin, den1);  bin /= g;  den1 /= g;
    g = seq_gcd(bin, den2);        bin /= g;  den2 /= g;

    /* Reduce the factor ‘num’ with remaining denominators */
    g = seq_gcd(num, den1);        num /= g;  den1 /= g;
    g = seq_gcd(num, den2);        num /= g;  den2 /= g;

    /* At this point den1 and den2 should be 1.  The product fits into seq_t. */
    return bin * num;
}

/* Public sequence function a(idx_t n) */
seq_t a(idx_t n)
{
    /* Find row r by simple incremental search (r is O(√n)). */
    idx_t r = 0;
    /* condition: (r+1)*(r+2)/2 ≤ n */
    while (( (__int128)(r + 1) * (r + 2) ) <= (__int128)2 * n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;
    return T(r, k);
}

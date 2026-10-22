/*
 * A391048  Triangle read by rows: T(n,k) = binomial(n+1,k+1) *
 *          binomial(5*n-4*k+1,k) / (n+1), 0<=k<=n.
 * https://oeis.org/A391048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A391048
   a(n) = T(row, col) where T(n,k) = binomial(n+1,k+1) * binomial(5*n-4*k+1,k) / (n+1)
   The index n counts the elements of the triangle row‑wise. */

/* required utilities */
#include "../utils/binomial.h"   /* static seq_t binomial(idx_t n, idx_t k) */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/floor_sqrt.h" /* seq_t floor_sqrt(seq_t x) */

/* convert a flat index to the corresponding row and column of the triangle   */
static void
index_to_rowcol(idx_t idx, idx_t *row, idx_t *col)
{
    /* solve r(r+1)/2 <= idx < (r+1)(r+2)/2  using integer sqrt   */
    seq_t val = (seq_t)8 * (seq_t)idx + (seq_t)1;          /* 8*idx + 1   */
    seq_t s   = floor_sqrt(val);                          /* floor sqrt  */
    *row = ((idx_t)s - (idx_t)1) / (idx_t)2;              /* r = floor((s-1)/2) */
    idx_t start = *row * (*row + (idx_t)1) / (idx_t)2;    /* first index in this row */
    *col = idx - start;
}

seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_rowcol(n, &r, &c);

    /* compute T(r,c) */
    seq_t bin1 = binomial(r + (idx_t)1, c + (idx_t)1);   /* binomial(r+1, c+1) */
    idx_t m = (idx_t)5 * r - (idx_t)4 * c + (idx_t)1;    /* m = 5*r-4*c+1   */
    seq_t bin2 = binomial(m, c);                         /* binomial(m, c)  */

    seq_t denom = (seq_t)(r + (idx_t)1);                 /* denominator (r+1) */

    /* reduce fraction bin1*bin2 / denom to avoid overflow */
    seq_t g1 = gcd(bin1, denom);
    bin1 /= g1;
    denom /= g1;

    seq_t g2 = gcd(bin2, denom);
    bin2 /= g2;
    denom /= g2;

    seq_t result = bin1 * bin2;                          /* denom should divide exactly */
    if (denom != (seq_t)1)
        result /= denom;

    return result;
}

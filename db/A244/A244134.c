/*
 * A244134  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n^n as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 28, i32 n <= 55, i64 n <= 153,
 *            i128 n <= 406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244134: Triangle read by rows – coefficients T(n,k) of
   n^n = Σ_{k=0..n} T(n,k) * binomial(n,k).
   T(n,k) = (-k)^(k-1)*(n+k)^(n-k) for k>0;   T(n,0)=0^n (1 if n=0 else 0).
   This function returns the sequence a(n) obtained by reading the
   triangle row‑wise: a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=3, ... */

#include <stdint.h>

/* small power function for seq_t with a non‑negative integer exponent */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) {
            res *= b;
        }
        e >>= 1;
        if (e) {
            b *= b;
        }
    }
    return res;
}

/* find row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t row_index(idx_t n)
{
    idx_t r = 0;
    /* linear search is cheap: row grows like sqrt(2*n) */
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t r = row_index(n);
    idx_t start = (r * (r + 1)) / 2;   /* first index of this row */
    idx_t c = n - start;               /* column within the row */

    /* T(r,0) */
    if (c == 0) {
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* compute T(r,c) = (-c)^(c-1) * (r+c)^(r-c) */
    seq_t pow1 = power_seq(-(seq_t)c, c - 1);
    seq_t pow2 = power_seq((seq_t)(r + c), r - c);
    return pow1 * pow2;
}

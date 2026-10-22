/*
 * A391610  Triangle T(n,k) read by rows: T(n,k) is the coefficient of x^k of
 *          the monic polynomial (1+x)^n + ((2*(1+x))^n - (2+x)^n) / x.
 * https://oeis.org/A391610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T(n,k) row‑by‑row, *
 *            where * * T(n,k) = [x^k] (1+x)^n + ((2*(1+x))^n - (2+x)^n) / x .
 *            * * The function below computes a(n) for any non‑negative n. *
 *            It uses the following helper routines: * - binom(idx_t a, idx_t
 *            b) -> C(a,b) * - seq_pow(seq_t base, idx_t...
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row r such that
       r*(r+1)/2 <= n < (r+1)*(r+2)/2
       For n up to ~10000 r is at most 141, so a simple loop is fast enough.
    */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    /* Column index within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;               /* 0 <= c <= r */

    /* The last entry of each row is always 1 (the polynomial is monic). */
    if (c == r) {
        return (seq_t)1;
    }

    /* Compute the two needed binomial coefficients. */
    seq_t bin1 = binom(r, c);          /* C(r,c)   */
    seq_t bin2 = binom(r, c + 1);      /* C(r,c+1) */

    /* Powers of two needed in the closed‑form formula. */
    seq_t two_nkc1 = seq_pow((seq_t)2, r - c - 1);  /* 2^(r-c-1)   */
    seq_t two_kp1  = seq_pow((seq_t)2, c + 1);     /* 2^(c+1)     */
    seq_t sub      = two_kp1 - (seq_t)1;           /* 2^(c+1)-1   */

    /* Compute the product part: C(r,c+1) * 2^(r-c-1) * (2^(c+1)-1) */
    seq_t prod = seq_mul(bin2, two_nkc1);
    prod = seq_mul(prod, sub);

    /* Final result: C(r,c) + product part */
    return bin1 + prod;
}

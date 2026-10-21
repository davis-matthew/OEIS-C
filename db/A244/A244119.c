/*
 * A244119  Triangle read by rows: terms of a binomial decomposition of 1 as
 *          Sum(k=0..n)T(n,k).
 * https://oeis.org/A244119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(row, col) where * T(r,0) = 0^r (0^0 = 1) * T(r,k) =
 *            (k+1)^(k-1) * (-k)^(r-k) * binomial(r,k) for k>0 * * Row is the
 *            unique r with r(r+1)/2 ≤ n < (r+1)(r+2)/2, and * col = n –
 *            r(r+1)/2
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 60, i64 n <= 145,
 *            i128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer power: computes base^exp using repeated squaring. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

/* Binomial coefficient C(n,k) computed without overflow in 64‑bit
 * by using 128‑bit intermediate values whenever possible. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (long long)(n - k + i);
        res = res / (long long)i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* Find row r: smallest r with r(r+1)/2 > n, then r--. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t start = r * (r + 1) / 2;   /* first index of this row   */
    idx_t k    = n - start;          /* column within the row     */

    /* Special column 0 */
    if (k == 0)
        return (r == 0) ? (seq_t)1 : (seq_t)0;

    /* Compute T(r,k) = binomial(r,k) * (k+1)^(k-1) * (-k)^(r-k)  */
    seq_t bin = binom(r, k);
    seq_t p1  = pow_seq((seq_t)(k + 1), k - 1);
    seq_t p2  = pow_seq(-(seq_t)k, r - k);

    return bin * p1 * p2;
}

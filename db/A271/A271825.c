/*
 * A271825  Triangle read by rows: T(n,m) =
 *          (-1)^(n-m-1)*m*binomial(2*n-3*m-1,n-m-1)/(n-m), T(n,n)=1.
 * https://oeis.org/A271825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,m) read by rows, with T(n,m) = (-1)^(n-m-1) * m *
 *            binomial(2n-3m-1, n-m-1)/(n-m), T(n,n) = 1
 * Range:     i8 n <= 15, i16 n <= 45, i32 n <= 231, i64 n <= 703,
 *            i128 n <= 2485, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative computation of C(n,k) for 0 ≤ k ≤ n (n,k unsigned) */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;            /* exact division */
    }
    return res;
}

/* binomial for possibly negative n and k ≥ 0:
   binom(n,k) = (-1)^k * binom(k-n-1,k) when n < 0 */
static seq_t binom_signed(seq_t n, idx_t k)
{
    if (k == (idx_t)0) return (seq_t)1;
    if ((long long)n >= 0LL) {
        return binom_calc((idx_t)n, k);
    } else {
        long long a = -(long long)n;          /* a = -n > 0 */
        idx_t top = (idx_t)(a - 1) + k;       /* k + a - 1 */
        seq_t val = binom_calc(top, k);
        if (k & 1u) return -val;
        return val;
    }
}

/* Triangle entry T(r,m) for 1 ≤ m ≤ r */
static seq_t triangle_entry(idx_t r, idx_t m)
{
    if (m == r) return (seq_t)1;

    idx_t diff = r - m;                       /* ≥ 1 */
    seq_t sign = ((diff - 1) & 1u) ? (seq_t)-1 : (seq_t)1;
    seq_t col  = (seq_t)m;
    idx_t bottom = diff - 1;                  /* r-m-1 ≥ 0 */

    seq_t binom_coeff;
    if (bottom == (idx_t)0) {
        binom_coeff = (seq_t)1;
    } else {
        long long top_signed = 2LL * (long long)r - 3LL * (long long)m - 1LL;
        if (top_signed >= 0LL) {
            binom_coeff = binom_calc((idx_t)top_signed, bottom);
        } else {
            binom_coeff = binom_signed((seq_t)top_signed, bottom);
        }
    }

    seq_t diffSeq = (seq_t)diff;               /* convert to signed for division */
    return sign * col * binom_coeff / diffSeq;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;        /* undefined for offset 0 */

    /* find row r such that r(r+1)/2 ≥ n */
    idx_t r = (idx_t)1;
    while (r * (r + 1) / 2 < n) ++r;

    /* column index within the row (1‑based) */
    idx_t start_of_row = (r - 1) * r / 2 + 1;
    idx_t m = n - start_of_row + 1;

    return triangle_entry(r, m);
}

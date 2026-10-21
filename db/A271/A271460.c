/*
 * A271460  Triangle read by rows: T(n,m) =
 *          (m/(n-m))*Sum_{k=1..n-m}((-1)^k*binomial(m-1,k-1)*binomial(3*(n-m)-k-1,n-m-k)),
 *          T(n,n)=1.
 * https://oeis.org/A271460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(1,1)=1; for n>m, T(n,m)=
 *            (m/(n-m))*Sum_{k=1..n-m}((-1)^k*binomial(m-1,k-1)*binomial(3*(n-m)-k-1,n-m-k));
 *            The sequence a(n) is the triangle read by rows with offset 1
 * Range:     i8 n <= 15, i16 n <= 36, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial C(n,k) for non‑negative integers,
   safe for signed seq_t by casting operands before each operation   */
static inline seq_t binom_mult(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;      /* symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - i + 1) / i;   exact division each step   */
        res = (res * (seq_t)(n - i + 1)) / (seq_t)i;
    }
    return res;
}

/* smallest row index r such that r*(r+1)/2 ≥ pos (pos is 1‑based) */
static inline idx_t row_of(idx_t pos)
{
    idx_t r = 1;
    while (r * (r + 1) / 2 < pos) ++r;
    return r;
}

/* compute T(n,m) for 1 ≤ m ≤ n, using the definition above */
static inline seq_t triangle_T(idx_t n, idx_t m)
{
    if (m == n) return 1;

    /* compute sum_{k=1..n-m} */
    seq_t sum = 0;
    idx_t diff_idx = n - m;                /* n-m is small (≤ n) */
    for (idx_t k = 1; k <= diff_idx; ++k) {
        seq_t b1 = binom_mult(m - 1, k - 1);
        seq_t b2 = binom_mult(3 * diff_idx - k - 1, diff_idx - k);
        seq_t term = b1 * b2;
        if (k & 1) sum -= term;
        else       sum += term;
    }

    /* T(n,m) = (m * sum) / (n-m)  (exact division) */
    seq_t prod = (seq_t)m * sum;
    seq_t diff = (seq_t)(diff_idx);        /* signed to avoid unsigned truncation */
    return prod / diff;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1, but not used */

    idx_t r = row_of(n);                 /* row in the triangle */
    idx_t start = (r - 1) * r / 2 + 1;   /* first index of that row (1‑based) */
    idx_t m = n - start + 1;             /* column within the row (1‑based) */

    return triangle_T(r, m);
}

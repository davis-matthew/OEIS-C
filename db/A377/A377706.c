/*
 * A377706  G.f. A(x) satisfies A(x) = 1 + x/A(x)^3 * (1 - A(x) + A(x)^4).
 * https://oeis.org/A377706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Σ_{k=0..n} (-1)^k * binomial(n,k) *
 *            binomial(n-4k, n-k-1) (n>0), a(0)=1 The second binomial is a
 *            generalized binomial that allows negative upper arguments. For
 *            k>n-1 the term is zero
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t, seq_t types from the build */

static seq_t binom_unsigned(idx_t n, idx_t k)
/* ordinary binomial n choose k (n,k ≥ 0) */
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;              /* use symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);        /* r *= n-k+i */
        r = r / (seq_t)i;                  /* exact division */
    }
    return r;
}

static seq_t binom_general(int64_t n, idx_t k)
/* generalized binomial allowing negative upper argument n */
{
    if (k == 0) return 1;
    if (n >= 0 && (idx_t)n < k) return 0;   /* ordinary n choose k for n < k */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - (int64_t)i + 1); /* multiply by n-i+1 */
        r = r / (seq_t)i;                   /* exact division */
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; ; ++k) {
        int64_t b = (int64_t)n - (int64_t)k - 1;  /* n-k-1 */
        if (b < 0) break;                        /* further k yield negative b */

        seq_t c1 = binom_unsigned(n, k);          /* binom(n,k)   */
        int64_t m = (int64_t)n - 4LL * (int64_t)k;/* n-4k         */
        seq_t c2 = binom_general(m, (idx_t)b);   /* binom(n-4k, b) */

        seq_t term = c1 * c2;                    /* product */
        if (k & 1ULL) term = -term;             /* (-1)^k factor */
        sum += term;
    }

    return sum / (seq_t)n;                       /* divide by n, exact */
}

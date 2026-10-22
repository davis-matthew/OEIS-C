/*
 * A371890  G.f. A(x) satisfies A(x) = 1 - x/A(x)^3 * (1 - A(x) - A(x)^4).
 * https://oeis.org/A371890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Sum_{k=0..n} binomial(n,k) * binomial(n-4*k,
 *            n-k-1) (a(0)=1)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 19, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code implements the formula directly, using a custom binomial routine that
   handles negative upper arguments in the sense binom(-m,k)=(-1)^k*binom(k+m-1,k). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Compute binomial(u,l) where u may be negative (int64_t).  l is guaranteed >=0. */
static seq_t binom_general_signed(int64_t u, idx_t l)
{
    if (l == 0) return (seq_t)1;
    if (u >= 0) {
        if ((idx_t)u < l) return (seq_t)0;
        return binomial((idx_t)u, l);
    }
    /* u < 0: use generalised binomial coefficient */
    int64_t m = -u;                 /* m > 0 */
    seq_t res = binomial((idx_t)(m + l - 1), l);
    if (l & 1) res = -res;
    return res;
}

/* Compute a single term a(n).  For n == 0 the value is 1. */
static seq_t compute_a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    seq_t Cnk = 1;                 /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        int64_t u = (int64_t)n - 4 * (int64_t)k;
        int64_t l = (int64_t)n - (int64_t)k - 1;

        seq_t B = 0;
        if (l >= 0)
            B = binom_general_signed(u, (idx_t)l);

        sum += Cnk * B;

        if (k < n) {
            Cnk = Cnk * ((seq_t)n - (seq_t)k) / (seq_t)(k + 1);
        }
    }

    return sum / (seq_t)n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    return compute_a(n);
}

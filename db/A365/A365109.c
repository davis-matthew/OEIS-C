/*
 * A365109  G.f. satisfies A(x) = 1 + x / (1 + x*A(x))^2.
 * https://oeis.org/A365109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..n} (-1)^(n-k) * C(n-k+1,k) * C(n+k-1,n-k)/(n-k+1)
 *            for A365109. Computed with a recurrence that needs only integer
 *            arithmetic
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External types supplied by the build. */
extern seq_t a(idx_t n);

/* The function itself. */
seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 1 by construction. */
    if (n == 0u) {
        return (seq_t)1;
    }

    seq_t total = 0;

    /* k = 1 term:  t1 = (-1)^(n-1) * n */
    seq_t sign = ((n - 1u) & 1u) ? (seq_t)-1 : (seq_t)1;
    seq_t term = sign * (seq_t)n;          /* t1 */
    total += term;
    seq_t prev = term;                     /* prev holds t_{k-1} */

    for (idx_t k = 2u; k <= n; ++k) {
        /* Compute numerator = prev * (n + k - 1) * (n - 2k + 3) * (n - 2k + 2) */
        seq_t num1 = (seq_t)(n + k - 1u);
        seq_t num2 = (seq_t)(n - 2 * k + 3u);
        seq_t num3 = (seq_t)(n - 2 * k + 2u);

        seq_t numer = prev * num1;
        numer *= num2;
        numer *= num3;

        /* Denominator = k * (2k - 1) * (2k - 2) */
        seq_t denom = (seq_t)(k) * (seq_t)(2 * k - 1u) * (seq_t)(2 * k - 2u);

        /* t_k = - (numer / denom) */
        seq_t next = numer / denom;
        next = -next;                      /* multiply by -1 */

        prev = next;
        total += next;
    }

    return total;
}

/*
 * A339028  Triangle T(n,m) =
 *          3*(m+1)*C(n+m+3,m)*C(2*n+2,n-m)/((n+m+3)*C(n+1,m)).
 * https://oeis.org/A339028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A339028 – Triangle T(n,m) = 3*(m+1)*C(n+m+3,m)*C(2*n+2,n-m) /
   ((n+m+3)*C(n+1,m))  (row‑major enumeration; a(0)=1) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"    /* provides binomial(idx_t n, idx_t k) */

/* Reduce a fraction <num/den> by multiplying it with the fraction a/b. */
static void mul_reduce(seq_t *num, seq_t *den, seq_t a, seq_t b)
{
    seq_t g1 = gcd(*num, b);
    *num /= g1;
    b /= g1;
    seq_t g2 = gcd(*den, a);
    *den /= g2;
    a /= g2;
    *num *= a;
    *den *= b;
}

seq_t a(idx_t n)
{
    /* Find the row r and column m for the given index n
       (row‑major enumeration of the triangle). */
    idx_t r = 0;
    /* Solve r(r+1)/2 <= n < (r+1)(r+2)/2  */
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    idx_t m = n - r * (r + 1) / 2;   /* 0 <= m <= r */

    /* Build the fraction step by step, cancelling factors to avoid
       overflow in any of the supported seq_t sizes. */
    seq_t num = 1;
    seq_t den = 1;

    /* 3*(m+1) */
    mul_reduce(&num, &den, (seq_t)(3 * (m + 1)), 1);

    /* C(n+m+3, m)  where n = r */
    seq_t bin1 = binomial(r + m + 3, m);
    mul_reduce(&num, &den, bin1, 1);

    /* C(2*n+2, n-m)  where n = r */
    seq_t bin2 = binomial(2 * r + 2, r - m);
    mul_reduce(&num, &den, bin2, 1);

    /* (n+m+3) in the denominator */
    mul_reduce(&num, &den, 1, (seq_t)(r + m + 3));

    /* C(n+1, m)  where n = r */
    seq_t bin3 = binomial(r + 1, m);
    mul_reduce(&num, &den, 1, bin3);

    /* The fraction is now fully reduced; divide to obtain the integer. */
    return num / den;
}

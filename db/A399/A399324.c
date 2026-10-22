/*
 * A399324  a(n) = (1/(n + 2)) * Sum_{k=0..n} binomial(2*k, k) * binomial(n+2,
 *          k+1). Row sums of A106534.
 * https://oeis.org/A399324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/(n + 2)) * Sum_{k=0..n} binomial(2*k, k) *
 *            binomial(n+2, k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

static seq_t mul_div_exact(seq_t a, seq_t b, seq_t den)
{
    seq_t g = gcd(a, den);
    a /= g;
    den /= g;
    g = gcd(b, den);
    b /= g;
    den /= g;
    /* den should now be 1 */
    return a * b;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 directly to avoid loop overhead */
    if (n == 0)
        return 1;   /* (1/(0+2)) * binom(0,0)*binom(2,1) = 2/2 = 1 */

    idx_t m = n + 2;                     /* m = n + 2 */
    seq_t c1 = binomial(m, 1);           /* binom(n+2,1) = n+2 */
    seq_t c2 = 1;                        /* binom(0,0) = 1 */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = c2 * c1 */
        sum += c2 * c1;

        if (k == n) break;               /* finished loop */

        /* Update c1 : binom(m, k+2) from binom(m, k+1) */
        idx_t r = k + 1;                 /* current r = k+1 */
        seq_t num1 = m - r;              /* m - r  */
        seq_t den1 = r + 1;              /* r + 1  */
        c1 = mul_div_exact(c1, num1, den1);

        /* Update c2 : binom(2(k+1), k+1) from binom(2k, k) */
        seq_t num2 = 4 * k + 2;          /* 4k + 2 */
        seq_t den2 = k + 1;              /* k + 1 */
        c2 = mul_div_exact(c2, num2, den2);
    }

    return sum / (n + 2);
}

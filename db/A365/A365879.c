/*
 * A365879  Expansion of (1/x) * Series_Reversion( x*(1+x)^3*(1-x)^5 ).
 * https://oeis.org/A365879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(3*n+k+2, k) *
 *            binomial(3*n-2*k+1, n-2*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/binomial.h"

/* Multiply: base * mult / div with gcd reductions to avoid overflow.
   All arguments and the result are of type seq_t.                */
static seq_t mul_div(seq_t base, seq_t mult, seq_t div)
{
    seq_t g = gcd(base, div);
    if (g != 1) {
        base /= g;
        div  /= g;
    }
    g = gcd(mult, div);
    if (g != 1) {
        mult /= g;
        div  /= g;
    }
    /* After reducing, div should be 1.  If not, the division is exact. */
    return (base * mult) / div;
}

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    idx_t m = n / 2;               /* floor(n/2) */
    seq_t bin1 = 1;                /* k = 0 term of C(3*n+k+2, k)  */
    seq_t bin2 = binomial(3*n + 1, n);   /* k = 0 term of C(3*n-2*k+1, n-2*k) */

    for (idx_t k = 0; k <= m; ++k) {
        /* Add the current term */
        sum += bin1 * bin2;

        if (k == m) break;        /* No more terms */

        /* Update bin1: bin1_{k+1} = bin1_k * (3*n + k + 3) / (k+1)  */
        seq_t num1 = (seq_t)(3 * (uint64_t)n + k + 3);
        seq_t den1 = (seq_t)(k + 1);
        bin1 = mul_div(bin1, num1, den1);

        /* Update bin2: bin2_{k+1} = bin2_k * (B_k*(B_k-1)) / (A_k*(A_k-1))   */
        idx_t Bk = n - 2 * k;
        idx_t Ak = 3 * n - 2 * k + 1;
        seq_t num2 = (seq_t)(Bk * (Bk - 1));
        seq_t den2 = (seq_t)(Ak * (Ak - 1));
        bin2 = mul_div(bin2, num2, den2);
    }

    /* Divide by (n+1) */
    return sum / ((seq_t)n + 1);
}

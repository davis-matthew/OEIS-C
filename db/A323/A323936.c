/*
 * A323936  Isomers of deoxyalditols.
 * https://oeis.org/A323936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^max(n-4, -2)*(n+2) + 2^((n-5)/2)*(n mod 2)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special cases: a(1)=1, a(2)=1, a(3)=3. For n≥4 use the formula directly. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle small n explicitly to avoid fractional exponents. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* n ≥ 4 */
    /* term1 = 2^(n-4) * (n+2) */
    seq_t term1 = big_pow((seq_t)2, (seq_t)(n - 4));
    term1 *= (seq_t)(n + 2);

    /* term2 = 2^((n-5)/2) if n is odd, else 0 */
    seq_t term2 = 0;
    if (n & 1U) {          /* n is odd */
        term2 = big_pow((seq_t)2, (seq_t)((n - 5) / 2));
    }

    return term1 + term2;
}

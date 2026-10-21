/*
 * A249848  Number of length 4+4 0..n arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..7} c_i * binom(n-i+8, 8) * with coefficients c
 *            = {10, 563, 5535, 15390, 14224, 4287, 311} . * This follows from
 *            the generating function * G(x) = x*(10 + 563*x + 5535*x^2 +
 *            15390*x^3 + 14224*x^4 + 4287*x^5 + 311*x^6) / (1 - x)^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom8(idx_t N)
{
    if (N < 8)
        return (seq_t)0;
    seq_t res = 1;
    for (idx_t j = 1; j <= 8; ++j) {
        res *= (seq_t)(N - 8 + j);
        res /= (seq_t)j;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1, return 0 as safe default */

    static const idx_t coeff[8] = {
        0ULL,      /* unused 0‑th index */
        10ULL,     /* i = 1  → coefficient 10   */
        563ULL,    /* i = 2  → 563   */
        5535ULL,   /* i = 3  → 5535  */
        15390ULL,  /* i = 4  → 15390 */
        14224ULL,  /* i = 5  → 14224 */
        4287ULL,   /* i = 6  → 4287  */
        311ULL     /* i = 7  → 311   */
    };

    seq_t result = 0;
    for (idx_t i = 1; i <= 7; ++i) {
        idx_t N = n - i + 8;          /* N = n - i + 8 */
        seq_t term = binom8(N);       /* binom(N, 8) */
        if (term) {
            result += (seq_t)coeff[i] * term;
        }
    }
    return result;
}

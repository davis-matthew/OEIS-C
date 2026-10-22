/*
 * A383343  a(n) = 3^n - 3*binomial(n,3) - 3*binomial(n,2) - 2*n - 1.
 * https://oeis.org/A383343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 3*binomial(n,3) - 3*binomial(n,2) - 2*n - 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t val = pow3(n);

    if (n >= 3) {
        val -= 3 * binomial(n, 3);
    }
    if (n >= 2) {
        val -= 3 * binomial(n, 2);
    }
    val -= ((seq_t)n) * 2;
    val -= 1;

    return val;
}

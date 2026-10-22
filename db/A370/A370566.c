/*
 * A370566  Size of the group Q_5*_/(Q_5*)^n, where Q_5 is the field of 5-adic
 *          numbers.
 * https://oeis.org/A370566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * 5^e * gcd(4, n'), where n = 5^e * n' and 5 ∤ n'
 * Range:     u8 n <= 19, u16 n <= 499, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safeguard, not part of OEIS offset */
    /* Determine exponent e of 5 in n */
    idx_t temp = n;
    idx_t e = 0;
    while (temp % 5 == 0) {
        temp /= 5;
        e++;
    }
    /* temp now equals n' (coprime to 5) */
    seq_t nprime = (seq_t)temp;
    /* Compute gcd(4, n') */
    seq_t g = gcd((seq_t)4, nprime);
    /* Compute 5^e */
    seq_t pow5 = 1;
    for (idx_t i = 0; i < e; i++) {
        pow5 *= (seq_t)5;
    }
    /* a(n) = n * 5^e * gcd(4, n') */
    seq_t result = (seq_t)n;
    result *= pow5;
    result *= g;
    return result;
}

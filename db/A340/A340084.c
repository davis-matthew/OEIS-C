/*
 * A340084  a(n) = gcd(n-1, A336466(n)); Odd part of A340081(n).
 * https://oeis.org/A340084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd_part(gcd(phi(n), n-1)) (with a(1) = 1)
 * Range:     u8 n <= 522, u16 n <= 9637, u32 n <= 9637, u64 n <= 9637,
 *            u128 n <= 9637, big any n
 * Helpers:   gcd, odd_part, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/odd_part.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t n_minus_1 = (seq_t)(n - 1);
    seq_t ph = phi(n);             /* Euler’s totient of n */
    seq_t g  = gcd(n_minus_1, ph);
    return odd_part(g);
}

/*
 * A331176  a(n) = n - n/gcd(n, phi(n)), where phi is Euler totient function.
 * https://oeis.org/A331176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - n/gcd(n, phi(n))
 * Range:     u8 n <= 273, u16 n <= 9957, u32 n <= 9957, u64 n <= 9957,
 *            u128 n <= 9957, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t phi_n = phi(n);
    seq_t g = gcd((seq_t)n, phi_n);
    seq_t div = (seq_t)n / g;
    return (seq_t)n - div;
}

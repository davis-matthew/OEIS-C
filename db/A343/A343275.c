/*
 * A343275  a(n) = |2*n - 10^length(n)|.
 * https://oeis.org/A343275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |2*n - 10^digits(n)|
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    // n is at least 1
    int dn = digits(n);          // number of decimal digits of n
    seq_t m   = pow10(dn);       // 10^digits(n)
    seq_t two_n = (seq_t)n * 2;  // 2*n
    return two_n > m ? two_n - m : m - two_n; // absolute difference
}

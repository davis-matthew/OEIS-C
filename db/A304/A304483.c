/*
 * A304483  a(n) = pi(n)*pi(2n), where pi is A000720: the prime counting
 *          function.
 * https://oeis.org/A304483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n) * pi(2n)
 * Range:     u8 n <= 39, u16 n <= 1155, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9672 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    seq_t pi_n  = prime_pi(n);
    seq_t pi_2n = prime_pi(n * ((idx_t)2));
    return pi_n * pi_2n;
}

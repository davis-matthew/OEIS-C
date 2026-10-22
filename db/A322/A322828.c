/*
 * A322828  a(n) is the concatenation of n and pi(n).
 * https://oeis.org/A322828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, pi(n))
 * Range:     u8 n <= 24, u16 n <= 540, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"
#include "../utils/pow10.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    seq_t pi = compute_pi(n);
    /* number of decimal digits in pi(n); 0 counts as 1 digit */
    uint64_t d = (pi == 0 ? 1 : digit_count((idx_t)pi));
    seq_t pow10d = pow10(d);
    return (seq_t)n * pow10d + pi;
}

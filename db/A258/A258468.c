/*
 * A258468  a(n) = lcm(n, n - tau(n)).
 * https://oeis.org/A258468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n, n - tau(n)) where tau(n) = number of divisors of n
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   _tau_uint64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/_tau_uint64.h"

seq_t a(idx_t n)
{
    seq_t un   = (seq_t)n;
    seq_t tau  = _tau_uint64((uint64_t)n);
    seq_t diff = un - tau;      /* safe: tau(n) <= n for n >= 1 */

    if (diff == (seq_t)0)
        return (seq_t)0;        /* lcm(n,0) = 0 */

    seq_t g = gcd(un, diff);
    /* lcm = n/g * (n - tau) */
    seq_t part = un / g;
    return part * diff;
}

/*
 * A283442  a(n) = lcm(n,5) / gcd(n,5).
 * https://oeis.org/A283442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,5) / gcd(n,5)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Since lcm(n,5) = n*5/gcd(n,5),  a(n) = n*5/(gcd(n,5)^2)
   which simplifies to: if 5|n then a(n)=n/5 else a(n)=5*n */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t g     = gcd(n_val, (seq_t)5);

    if (g == (seq_t)5) {
        return n_val / (seq_t)5;
    } else {
        return n_val * (seq_t)5;
    }
}

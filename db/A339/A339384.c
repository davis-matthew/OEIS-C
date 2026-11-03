/*
 * A339384  a(n) = Sum_{k=1..n} (lcm(n,k)/gcd(n,k) mod n).
 * https://oeis.org/A339384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (lcm(n,k)/gcd(n,k) mod n)
 * Range:     u8 n <= 35, u16 n <= 479, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 80 terms through n = 8377 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for 0, return 0 just in case */
    seq_t sum   = 0;
    seq_t seq_n = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t g    = gcd(seq_n, (seq_t)k);          /* gcd(n,k) */
        seq_t term = ((seq_n / g) * ((seq_t)k / g)) % seq_n;
        sum += term;
    }

    return sum;
}

/*
 * A352482  Denominator of (n-d)/n*d where d = A000005(n) is the number of
 *          divisors of n.
 * https://oeis.org/A352482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n * d) / gcd(n * d, n - d), where d = τ(n) is the number
 *            of divisors of n
 * Range:     u8 n <= 35, u16 n <= 4124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9576 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* τ(n) – number of divisors */
    uint64_t d = num_divisors(n);

    /* n * d */
    seq_t nd = (seq_t)n * (seq_t)d;

    /* n - d (always non‑negative for n >= 1) */
    seq_t diff = (seq_t)(n - d);

    /* gcd(n*d, n-d) */
    seq_t g = gcd(nd, diff);

    /* denominator after reduction */
    return nd / g;
}

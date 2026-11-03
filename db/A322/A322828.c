/*
 * A322828  a(n) is the concatenation of n and pi(n).
 * https://oeis.org/A322828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, pi(n))
 * Range:     u8 n <= 24, u16 n <= 540, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9350 (2026-11-01)
 * Helpers:   num_digits0, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/num_digits0.h"
#include "../utils/prime_pi.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t pi = prime_pi(n);
    /* number of decimal digits in pi(n); 0 counts as 1 digit */
    uint64_t d = (pi == 0 ? 1 : num_digits0((idx_t)pi));
    seq_t pow10d = pow10(d);
    return (seq_t)n * pow10d + pi;
}

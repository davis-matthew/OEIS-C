/*
 * A288801  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 507", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0) = 1 a(1) = 11 for odd n ≥ 3: a(n) = 10 * (10^n -
 *            1)/9 (decimal digits: n ones followed by a 0) for even n: let m
 *            = 2^⌊log2((n+2)/2)⌋, a(n) = (10^m - 1)/9 (decimal digits: m
 *            ones)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 125 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ilog2.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t ones_decimal(idx_t k)
{
    seq_t t = pow10(k);
    t -= 1;
    t /= 9;
    return t;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    if (n & 1) {                /* odd n ≥ 3 */
        seq_t t = pow10(n);
        t -= 1;
        t /= 9;
        t *= 10;
        return t;
    } else {                    /* even n */
        idx_t x = (n + 2) / 2;   /* x ≥ 1 */
        unsigned l = ilog2(x);
        idx_t m = (idx_t)1 << l; /* m = largest power of two ≤ x */
        return ones_decimal(m);
    }
}

/*
 * A279386  Decimal expansion of h^2, where h is the Planck constant in SI
 *          units.
 * https://oeis.org/A279386
 *
 * Offset:    -66    (a(-66) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) returns the n‑th decimal digit of h² (Planck constant
 *            squared: 4.39048056327210225 × 10⁻⁶⁷). For n < -66 or n > -49
 *            the value is 0
 * Range:     u8 n <= -49, u16 n <= -49, u32 n <= -49, u64 n <= -49,
 *            u128 n <= -49, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const char DIGITS[] = "439048056327210225";

seq_t a(idx_t n)
{
    if (n < -66) return (seq_t)0;
    if (n > -49) return (seq_t)0;
    int idx = (int)(n + 66);
    return (seq_t)(DIGITS[idx] - '0');
}

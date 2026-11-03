/*
 * A341953  Replace each digit d in the decimal representation of n with the
 *          digital root of d^n.
 * https://oeis.org/A341953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Replace each decimal digit d of n by the digital * root
 *            of dⁿ and concatenate the resulting digits
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9646 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for uint64_t, if needed */

/* Compute the digital root of dⁿ (modulo 9).  For d = 0 the result is 0;
 * otherwise the digital root is 1–9, with 9 representing a residue of 0
 * modulo 9.  The exponent n is given as idx_t.  The result is returned
 * as seq_t. */
static inline seq_t dr_pow(idx_t n, int d)
{
    if (d == 0) return 0;
    int base = d % 9;
    int r    = 1 % 9;
    idx_t e  = n;
    while (e) {
        if (e & 1U) r = (r * base) % 9;
        base = (base * base) % 9;
        e >>= 1;
    }
    return (seq_t)(r == 0 ? 9 : r);
}

/* Main sequence function.  idx_t n >= 1.  The result is constructed by
 * processing the decimal digits of n from most significant to least
 * significant, replacing each digit by the digital root of digitⁿ,
 * and concatenating the single‑digit results. */
seq_t a(idx_t n)
{
    /* n is at least 1; for n == 0 we simply return 0 (not used). */
    if (n == 0) return 0;

    /* Collect decimal digits of n in reverse order. */
    int digits[21];      /* uint64_t can have at most 20 decimal digits */
    int len = 0;
    idx_t tmp = n;
    while (tmp) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d  = digits[i];
        seq_t dr = dr_pow(n, d);
        result = result * 10 + dr;
    }
    return result;
}

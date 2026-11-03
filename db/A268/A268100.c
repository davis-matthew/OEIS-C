/*
 * A268100  a(n) = 2^((n-1) mod 2)*5*10^floor((n-1)/2).
 * https://oeis.org/A268100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^((n-1) mod 2)*5*10^floor((n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    /* the first term is special */
    if (n == 0)
        return 1;

    /* compute exponent of 10 */
    idx_t k = (n - 1) / 2;
    seq_t val = pow10(k);

    /* multiply by the appropriate factor (5 or 10) */
    if ((n - 1) & 1) /* odd (n-1) -> factor 10 */
        val *= 10;
    else            /* even (n-1) -> factor 5  */
        val *= 5;

    return val;
}

/*
 * A251984  Smallest number such that a carry occurs when adding it to n in
 *          decimal representation.
 * https://oeis.org/A251984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = (10 - d) * 10^p where p is the number of trailing zeros
 *            in n, and d is the first non‑zero decimal digit of n (from the
 *            right)
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 100 terms through n = 9927 (2026-11-01)
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
    if (n == 0) return 0;
    idx_t tmp = n;
    idx_t p   = 0;

    /* Count trailing zeros */
    while ((tmp % 10) == 0) {
        tmp /= 10;
        p++;
    }

    /* tmp % 10 now is a non‑zero digit (1..9) */
    unsigned int d = (unsigned int)(tmp % 10);

    seq_t pow = pow10(p);
    return ((seq_t)(10 - d)) * pow;
}

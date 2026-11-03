/*
 * A273774  Decimal expansion of Jevon's number.
 * https://oeis.org/A273774
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = the (n-9)th digit of the integer 8616460799 (digits
 *            counted from the most significant digit). For n beyond the 10th
 *            digit, a(n) = 0
 * Range:     u8 none, u16 none, u32 none, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: full, pass, 10 terms through n = 19 (2026-11-02)
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
    /* The required constant. */
    const seq_t N = (seq_t)8616460799ULL;

    if (n < 10)
        return (seq_t)0;

    /* Position of the desired digit (0 = most significant). */
    idx_t pos = n - 10;

    if (pos >= 10)
        return (seq_t)0;

    /* power = 10^(9 - pos)   10^9 for the most significant digit,
       10^0 for the least significant. */
    idx_t exp = 9 - pos;
    seq_t power = pow10(exp);

    seq_t digit = (N / power) % (seq_t)10;
    return digit;
}

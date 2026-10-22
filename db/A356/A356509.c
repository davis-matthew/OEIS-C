/*
 * A356509  (Negated) Decimal expansion of value of absolute zero in degrees
 *          Fahrenheit.
 * https://oeis.org/A356509
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = decimal digit of 459.67 at position n‑3 (0 for n ≥ 8),
 *            where the decimal expansion of absolute zero is 459.67 …
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* optional, for completeness */

seq_t a(idx_t n)
{
    /* Undefined for n < 3 – any value is acceptable. */
    if (n < 3)
        return 0;

    /* 459.67 without the decimal point. */
    const idx_t V = 45967;

    /* 10⁰ … 10⁴ for the digit extraction. */
    const idx_t pow10[5] = {1, 10, 100, 1000, 10000};

    idx_t k = n - 3;          /* position in the digit sequence 0‑4 */

    if (k >= 5)              /* all remaining digits are 0 */
        return 0;

    /* Extract the k‑th digit: V / 10^(4‑k)  (mod 10). */
    seq_t digit = (seq_t)((V / pow10[4 - k]) % 10);
    return digit;
}

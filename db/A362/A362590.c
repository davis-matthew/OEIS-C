/*
 * A362590  Decimal expansion of the conventional value of von Klitzing
 *          constant (R_{K-90}) in ohms (Omega).
 * https://oeis.org/A362590
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   a(n) = decimal digit of 25812.807 at position n-5, or 0 if n >
 *            12
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard for indices below the offset – behavior not specified */
    if (n < 5) return (seq_t)0;

    /* Digits of the conventional von Klitzing constant 25812.807 */
    static const seq_t digits[8] = {
        (seq_t)2, (seq_t)5, (seq_t)8, (seq_t)1,
        (seq_t)2, (seq_t)8, (seq_t)0, (seq_t)7
    };

    idx_t k = n - 5;          /* Zero‑based position in the digit array */
    if (k < 8) return digits[k];

    /* After the 807 digits, the decimal representation is infinite zeros */
    return (seq_t)0;
}

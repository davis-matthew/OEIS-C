/*
 * A322578  Decimal expansion of the Avogadro constant N_A in the 2019 SI
 *          system in units mol^(-1).
 * https://oeis.org/A322578
 *
 * Offset:    24    (a(24) is the first term)
 * Keywords:  nonn, easy, cons, fini, full
 * Formula:   a(n) = digit of the integer 602214076000000000000000 * at
 *            position (n-23). Positions 1–9 come from 602214076, * all later
 *            positions are zero
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The offset of this sequence is 24; a(24) is the first digit. */
seq_t a(idx_t n)
{
    /* offset is 24, so the first digit is at n = 24. */
    if (n < 24) {
        /* below the offset the value is not defined; return 0 as a fallback. */
        return (seq_t)0;
    }
    idx_t pos = n - 23;            /* 1‑based position in the decimal expansion */
    if (pos <= 9) {
        /* digits of 602214076 */
        static const seq_t digits[9] = {6, 0, 2, 2, 1, 4, 0, 7, 6};
        return digits[pos - 1];
    }
    /* all remaining positions are zero because the integer ends with 15 zeros */
    return (seq_t)0;
}

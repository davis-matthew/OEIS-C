/*
 * A261150  a(n) = 403185216600637 + (n-1)*2124513401010.
 * https://oeis.org/A261150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 403185216600637 + (n-1)*2124513401010
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t base = (seq_t)403185216600637ULL;
    const seq_t step = (seq_t)2124513401010ULL;

    /* If n < 1, undefined by OEIS; we return the first term for safety. */
    if (n < 1) {
        return base;
    }

    return base + ((seq_t)n - 1) * step;
}

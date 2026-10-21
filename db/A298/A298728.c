/*
 * A298728  EBCDIC codes for upper case letters.
 * https://oeis.org/A298728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn, word
 * Formula:   a(n) = n + 192 + (n >= 10 ? 7 : 0) + (n >= 19 ? 8 : 0)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the EBCDIC code for the n‑th upper‑case letter (A=1, B=2, …, Z=26) */
seq_t a(idx_t n)
{
    /* For safety, ignore n == 0 (below the sequence offset) */
    if (n == 0)
        return 0;
    return (seq_t)n + 192
           + (n >= 10 ? 7 : 0)
           + (n >= 19 ? 8 : 0);
}

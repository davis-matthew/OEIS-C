/*
 * A273867  Degrees of fundamental invariants of Weyl group W(E_6).
 * https://oeis.org/A273867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Degrees of fundamental invariants of Weyl groups W(E₆), W(E₇), W(E₈)
   concatenated repeatedly: 2,5,6,8,9,12 | 2,6,8,10,12,14,18 | 2,8,12,14,18,20,24,30
   (Pattern length = 21) */

seq_t a(idx_t n)
{
    static const seq_t pattern[21] = {
        2, 5, 6, 8, 9, 12,
        2, 6, 8,10,12,14,18,
        2, 8,12,14,18,20,24,30
    };
    return pattern[(n - 1) % 21];
}

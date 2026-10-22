/*
 * A321131  Values of m (mod 25), where A317905(m) = 1. Values of m (mod 25)
 *          such that V(m) = 1, where V(m) indicates the constant convergence
 *          speed of the tetration base m.
 * https://oeis.org/A321131
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = R[(n-2) mod 16], where R =
 *            [2,3,4,6,8,9,11,12,13,14,16,17,19,21,22,23]
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence repeats the 16 allowed residues modulo 25 for every block of 25 consecutive integers. */
seq_t a(idx_t n)
{
    /* The definition is only meaningful for n ≥ 2; values for n < 2 are unused. */
    static const seq_t R[16] = {2, 3, 4, 6, 8, 9, 11, 12, 13, 14, 16, 17, 19, 21, 22, 23};
    if (n < 2) return 0;                 /* undefined behavior is acceptable for n < 2 */
    return R[(n - 2) % 16];
}

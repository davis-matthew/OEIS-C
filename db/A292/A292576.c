/*
 * A292576  Permutation of the natural numbers partitioned into quadruples
 *          [4k-1, 4k-3, 4k-2, 4k], k > 0.
 * https://oeis.org/A292576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)/4)*4 + (n-1)%4 == 0 ? 3 : (n-1)%4 == 1 ? 1 :
 *            (n-1)%4 == 2 ? 2 : 4
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* out of bounds – not part of the sequence */
        return 0;

    idx_t shift = n - 1;            /* shift to 0‑based index */
    idx_t q = shift / 4;            /* quadruple number (k-1) */
    idx_t r = shift % 4;            /* position inside quadruple */

    seq_t base = ((seq_t)q) * 4;    /* base value 4*(k-1) */

    /* offset values for the four positions */
    static const seq_t offset[4] = {3, 1, 2, 4};

    return base + offset[r];
}

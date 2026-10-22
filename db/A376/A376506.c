/*
 * A376506  Natural numbers whose iterated squaring modulo 100 eventually
 *          settles at the attractor 1.
 * https://oeis.org/A376506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100 * floor((n-1)/8) + {1,7,43,49,51,57,93,99}[ (n-1) mod
 *            8 ]
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned base[8] = {1U, 7U, 43U, 49U, 51U, 57U, 93U, 99U};
    if (n == 0) return 0;                  /* offset 1, behaviour below irrelevant */
    idx_t k    = n - 1;
    idx_t div  = k / 8;
    unsigned rem = (unsigned)(k % 8);
    seq_t result = (seq_t)base[rem] + (seq_t)div * 100;
    return result;
}

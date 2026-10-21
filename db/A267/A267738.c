/*
 * A267738  Number of nX4 arrays of permutations of 4 copies of 0..n-1 with
 *          every element equal to or 1 greater than any west or northeast
 *          neighbors modulo n and the upper left element equal to 0.
 * https://oeis.org/A267738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 253*2^{n-5} - 4 for n >= 5, with a(1)=1, a(2)=35,
 *            a(3)=57, a(4)=123
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    static const seq_t first5[] = {1, 35, 57, 123, 249};
    if (n <= 5) return first5[n - 1];

    /* For n >= 6, use the closed‑form a(n) = 253*2^{n-5} - 4 */
    seq_t res = (seq_t)253;
    res <<= (n - 5); /* shift left by (n-5) places, i.e., multiply by 2^{n-5} */
    res -= (seq_t)4;
    return res;
}

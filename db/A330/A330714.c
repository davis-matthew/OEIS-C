/*
 * A330714  For any n >= 0 with binary expansion Sum_{k=0..w} b_k * 2^k, let
 *          h(n) = Sum_{k=0..w} b_k * i^k (where i denotes the imaginary
 *          unit); a(n) is the square of the modulus of h(n).
 * https://oeis.org/A330714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |∑_{k} b_k * i^k|^2 where b_k are the bits of n in binary
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;          /* copy of the index to iterate over bits */
    int re = 0;           /* real part accumulator (signed) */
    int im = 0;           /* imaginary part accumulator (signed) */
    int mod = 0;          /* current bit position modulo 4 */

    while (x != (idx_t)0) {
        if (x & (idx_t)1) {
            /* Update real and imaginary parts depending on position mod 4 */
            switch (mod) {
                case 0: re += 1; break;   /* i^0 = 1   */
                case 1: im += 1; break;   /* i^1 = i   */
                case 2: re -= 1; break;   /* i^2 = -1  */
                case 3: im -= 1; break;   /* i^3 = -i  */
            }
        }
        x >>= 1;              /* move to next bit */
        mod = (mod + 1) & 3;   /* keep mod 4 */
    }

    /* Compute |h(n)|^2 = re^2 + im^2, cast to seq_t for final result */
    seq_t result = (seq_t)re * (seq_t)re + (seq_t)im * (seq_t)im;
    return result;
}

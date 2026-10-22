/*
 * A335110  a(n) = Sum_{k=0..n} (Stirling1(n,k) mod 2) * k.
 * https://oeis.org/A335110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*n + 1)/2) * 2^(A000120(floor(n/2)) - 1)
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t = floor((3*n + 1)/2) as seq_t */
    seq_t t = ((seq_t)n * 3 + 1) / 2;

    /* m = floor(n/2) */
    idx_t m = n >> 1;

    /* bitcount of m (A000120(m)) */
    unsigned bc = 0;
    while (m) {
        bc++;
        m &= m - 1;
    }

    if (bc == 0)          /* exponent is -1, divide by 2 */
        return t / 2;
    else                  /* exponent is (bc-1) >= 0, shift left */
        return t << (bc - 1);
}

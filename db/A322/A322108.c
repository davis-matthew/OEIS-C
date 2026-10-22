/*
 * A322108  Distance of n-th iteration in an alternating rectangular spiral.
 * https://oeis.org/A322108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 - (-i)^n - i^n + 6*n - 2*n^2 + 2*n^3) / 8
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t c;
    switch (n & 3)
    {
        case 0: c = 0; break;
        case 1: c = 2; break;
        case 2: c = 4; break;
        case 3: c = 2; break;
        default: c = 0; /* unreachable */
    }
    seq_t num = ((seq_t)2 * nn * nn * nn)
              - ((seq_t)2 * nn * nn)
              + ((seq_t)6 * nn)
              + c;
    return num / 8;
}

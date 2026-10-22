/*
 * A319497  a(0)=0, a(3*n)=9*a(n), a(3*n+1)=9*a(n)+1, a(3*n+2)=9*a(n)+3.
 * https://oeis.org/A319497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(3*n)=9*a(n), a(3*n+1)=9*a(n)+1, a(3*n+2)=9*a(n)+3
 * Range:     u8 n <= 23, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t  res = 0;     /* accumulated result */
    seq_t  pow9 = 1;    /* 9^i for current ternary digit */
    idx_t  m = n;       /* working copy of the index */

    while (m > 0) {
        idx_t digit = m % 3;          /* extract next ternary digit (0,1,2) */
        seq_t add = (digit == 0) ? 0
                                : (digit == 1) ? 1
                                : 3;  /* map 2 -> 3 */
        res += add * pow9;
        pow9 *= 9;                    /* next power of 9 */
        m /= 3;                       /* shift to next digit */
    }

    return res;
}

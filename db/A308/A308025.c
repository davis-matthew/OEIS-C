/*
 * A308025  a(n) = n*(2*n - 3 - (-1)^n)*(5*n - 2 + (-1)^n)/16.
 * https://oeis.org/A308025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n - 3 - (-1)^n)*(5*n - 2 + (-1)^n)/16
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 762, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1   (so a(1) is the first term listed) */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t two_n = ((seq_t)n) << 1;       /* 2 * n   */
    seq_t five_n = (seq_t)n * 5;         /* 5 * n   */
    seq_t t1, t2;

    if (n & 1ULL) {                     /* odd n   */
        t1 = two_n - 2;                 /* 2*n - 2 */
        t2 = five_n - 3;                /* 5*n - 3 */
    } else {                            /* even n  */
        t1 = two_n - 4;                 /* 2*n - 4 */
        t2 = five_n - 1;                /* 5*n - 1 */
    }

    seq_t res = ((seq_t)n) * t1;        /* n * t1  */
    res = res * t2;                     /* n*t1*t2 */
    res = res >> 4;                     /* divide by 16 */

    return res;
}

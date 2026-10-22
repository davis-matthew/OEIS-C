/*
 * A330133  a(n) = (1/16)*(5 + (-1)^(1+n) - 4*cos(n*Pi/2) + 10*n^2).
 * https://oeis.org/A330133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5 + (-1)^(1+n) - 4*cos(n*Pi/2) + 10*n^2) / 16
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t num = 5; /* start with the constant 5 */

    /* add (-1)^(1+n): +1 for odd n, -1 for even n */
    if (n & 1ULL)      /* odd */
        num += 1;
    else               /* even */
        num -= 1;

    /* handle the -4*cos(n*Pi/2) part */
    /* cos values repeat 1,0,-1,0 for n mod 4 == 0,1,2,3 respectively */
    switch (n & 3ULL) {
        case 0:            /* cos =  1  => subtract 4 */
            num -= 4;
            break;
        case 2:            /* cos = -1  => add 4 */
            num += 4;
            break;
        case 1:            /* cos =  0  => nothing */
        case 3:            /* cos =  0  => nothing */
            break;
    }

    /* add the 10*n^2 term */
    seq_t n2 = (seq_t)n * (seq_t)n;   /* n squared (may wrap on tiny types) */
    num += 10 * n2;

    /* divide by 16; division is exact by construction */
    return num / 16;
}

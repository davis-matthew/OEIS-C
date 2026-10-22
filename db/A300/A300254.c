/*
 * A300254  a(n) = 25*(n + 1)*(4*n + 3)*(5*n + 4)/3.
 * https://oeis.org/A300254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*(n + 1)*(4*n + 3)*(5*n + 4)/3
 * Range:     u8 none, u16 n <= 4, u32 n <= 268, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the larger type first */
    seq_t nn = (seq_t)n;

    /* Compute the three factors separately */
    seq_t t1 = nn + 1;            /* n + 1          */
    seq_t t2 = 4 * nn + 3;        /* 4*n + 3        */
    seq_t t3 = 5 * nn + 4;        /* 5*n + 4        */

    /* Multiply all factors, divide by 3, then multiply by 25 */
    seq_t res = t1 * t2;          /* (n+1)*(4*n+3)  */
    res *= t3;                    /* (n+1)*(4*n+3)*(5*n+4) */
    res *= 25;                    /* *25            */
    res /= 3;                     /* /3             */

    return res;
}

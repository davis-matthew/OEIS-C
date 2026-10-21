/*
 * A245578  The number of permutations of {0,0,1,1,...,n-1,n-1} that begin
 *          with 0 and in which adjacent elements are adjacent mod n.
 * https://oeis.org/A245578
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3+5*(-1)^n+8*n+2*n^2)/4 for n>2; a(2)=1
 * Range:     u8 n <= 9, u16 n <= 221, u32 n <= 1006, u64 n <= 1006,
 *            u128 n <= 1006, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)           /* offset starts at 2 */
        return 1;        /* a(2)=1 */

    seq_t ns = (seq_t)n;                     /* promotion to seq_t */
    seq_t numer = 3 + 8 * ns;                /* 3 + 8*n */
    numer = numer + 2 * ns * ns;             /* + 2*n^2 */

    if ((n & 1ULL) == 0)                     /* even n: add 5 */
        numer = numer + 5;
    else                                     /* odd n: subtract 5 */
        numer = numer - 5;

    return numer / 4;
}

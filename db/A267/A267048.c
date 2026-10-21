/*
 * A267048  Number of OFF (white) cells in the n-th iteration of the "Rule 91"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=3. For odd n>1: a(n)=3. For even n>=4:
 *            a(n)=2*(n-2)
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage function */
seq_t a(idx_t n)
{
    /* handle the few special cases */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;

    /* odd indices (n>1) */
    if ( (n & 1) ) return 3;

    /* even indices >=4 */
    /* compute 2*(n-2) */
    idx_t m = n - 2;               /* m = n-2, fits in idx_t   */
    idx_t prod = 2 * m;             /* 2*m   (still idx_t)      */
    return (seq_t)prod;             /* truncate down to seq_t    */
}

/*
 * A256822  Number of length n+7 0..1 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A256822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 30 n^4 + 2895 n^3 - 5070 n^2 + 66584 n - 145920) /
 *            120 for n>5; a(1)=256, a(2)=512, a(3)=1024, a(4)=2048, a(5)=3728
 * Range:     u8 none, u16 n <= 5, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first five terms explicitly */
    if (n <= 5) {
        switch (n) {
            case 1: return (seq_t)256;
            case 2: return (seq_t)512;
            case 3: return (seq_t)1024;
            case 4: return (seq_t)2048;
            case 5: return (seq_t)3728;
        }
    }

    /* compute the general formula for n > 5 */
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;          /* n^2 */
    seq_t n3  = n2 * N;         /* n^3 */
    seq_t n4  = n3 * N;         /* n^4 */
    seq_t n5  = n4 * N;         /* n^5 */

    seq_t term1 = n5;
    seq_t term2 = n4 * (seq_t)30;      /* 30 n^4 */
    seq_t term3 = n3 * (seq_t)2895;     /* 2895 n^3 */
    seq_t term4 = n2 * (seq_t)5070;     /* 5070 n^2 */
    seq_t term5 = N  * (seq_t)66584;    /* 66584 n */
    seq_t const_term = (seq_t)145920;   /* 145920 */

    seq_t num = term1 + term2 + term3;  /* positive part */
    num -= term4;                       /* subtract 5070 n^2 */
    num += term5;                       /* add 66584 n */
    num -= const_term;                  /* subtract 145920 */

    return num / (seq_t)120;
}

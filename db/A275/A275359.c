/*
 * A275359  Maximum incarceration of numbers in an n X n X n number cubes with
 *          full incarceration volumes.
 * https://oeis.org/A275359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6-6*n^5+39*n^4-99*n^3+108*n^2-51*n+8)/2 for n>0,
 *            a(0)=0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t N = (seq_t)n;
    seq_t sq = N * N;        // n^2
    seq_t cu = sq * N;       // n^3
    seq_t p4 = cu * N;       // n^4
    seq_t p5 = p4 * N;       // n^5
    seq_t p6 = p5 * N;       // n^6

    seq_t poly = p6;
    poly -= (seq_t)6 * p5;
    poly += (seq_t)39 * p4;
    poly -= (seq_t)99 * cu;
    poly += (seq_t)108 * sq;
    poly -= (seq_t)51 * N;
    poly += 8;

    return poly / 2;
}

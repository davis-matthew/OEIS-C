/*
 * A293413  Least integer k such that k/n^2 > e.
 * https://oeis.org/A293413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(e*n^2)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  choose a fixed scaling factor D = 10^18
        N = floor(e*D) = 2718281828459045235
        Since n^2 < D for all n commonly tested (n <= 1e9),
        n^2/D < 1, so floor((N*n^2)/D) = floor(e*n^2). */
    const seq_t D = (seq_t)1000000000000000000ULL;          /* 1e18   */
    const seq_t N = (seq_t)2718281828459045235ULL;          /* floor(e*D) */

    seq_t sq = (seq_t)n * (seq_t)n;      /* n^2   */

    /* use unsigned __int128 for intermediate to avoid overflow
       (works both in C and C++). */
    unsigned __int128 prod = (unsigned __int128)sq * (unsigned __int128)N;
    unsigned __int128 val  = prod + ((unsigned __int128)D - 1);
    unsigned __int128 ceil = val / (unsigned __int128)D;

    return (seq_t)ceil;
}

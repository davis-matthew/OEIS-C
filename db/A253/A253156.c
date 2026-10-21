/*
 * A253156  Number of (n+1) X (5+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=181, a(2)=192, a(3)=201, a(n)=9*2^(n-1)+164 for n>=4
 *            (equivalently a(n)=3*a(n-1)-2*a(n-2) for n>5)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* explicit handling of the first three terms */
    if (n == 1) return (seq_t)181;
    if (n == 2) return (seq_t)192;
    if (n == 3) return (seq_t)201;

    /* compute 2^(n-1) by repeated doubling */
    seq_t power = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        power *= (seq_t)2;          /* power = 2^(n-1) */

    /* apply the closed‑form formula */
    power *= (seq_t)9;            /* 9*2^(n-1) */
    power += (seq_t)164;          /* 9*2^(n-1)+164 */

    return power;
}

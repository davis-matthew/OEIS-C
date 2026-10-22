/*
 * A303916  Constant term in the expansion of (Sum_{k=0..n} k*(x^k +
 *          x^(-k)))^3.
 * https://oeis.org/A303916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(n+1)*(n+2)*(2*n+1)/10
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 116, u64 n <= 9839,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
/* C implementation using 128‑bit intermediate for speed */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    unsigned __int128 prod = (unsigned __int128)(n - 1);
    prod *= n;
    prod *= (n + 1);
    prod *= (n + 2);
    prod *= ((unsigned __int128)2 * n + 1);
    prod /= 10;
    return (seq_t)prod;
}
#else
/* C++ implementation that works with arbitrary‑precision seq_t types */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t prod = (seq_t)(n - 1);
    prod *= (seq_t)n;
    prod *= (seq_t)(n + 1);
    prod *= (seq_t)(n + 2);
    seq_t two_n_plus_one = (seq_t)n;
    two_n_plus_one *= (seq_t)2;
    two_n_plus_one += (seq_t)1;
    prod *= two_n_plus_one;
    prod /= 10;
    return prod;
}
#endif

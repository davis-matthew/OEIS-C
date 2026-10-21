/*
 * A244882  Expansion of (1 + 2*x + 2*x^2) / (1 - x)^6.
 * https://oeis.org/A244882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+5,5) + 2*C(n+4,5) + 2*C(n+3,5) where C(k,5) = k! /
 *            (5! (k-5)!) for k >= 5, C(k,5)=0 otherwise
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom5(idx_t k)
{
    if (k < 5) return 0;
    /* product of (k-4)*(k-3)*(k-2)*(k-1)*k  divided by 120 */
    unsigned __int128 prod = 1;
    prod *= (unsigned __int128)(k - 4);
    prod *= (unsigned __int128)(k - 3);
    prod *= (unsigned __int128)(k - 2);
    prod *= (unsigned __int128)(k - 1);
    prod *= (unsigned __int128)k;
    prod /= 120;
    return (seq_t)prod;
}

seq_t a(idx_t n)
{
    /* compute binomial terms with 128-bit intermediate to avoid overflow */
    seq_t term1 = binom5(n + 5);
    seq_t term2 = 2 * binom5(n + 4);
    seq_t term3 = 2 * binom5(n + 3);
    return term1 + term2 + term3;
}

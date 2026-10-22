/*
 * A381912  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 / B(x) ), where
 *          B(x) is the g.f. of A001764.
 * https://oeis.org/A381912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+3*k+1,k) *
 *            binomial(3*n-k+1,n-k)/(n+3*k+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#if defined(__SIZEOF_INT128__)
#define HAVE_UINT128 1
#endif

/* Compute binomial(N,K) using the multiplicative formula.
   Works for seq_t <= 64‑bit (via 128‑bit intermediate) and for arbitrary
   precision seq_t. */
static seq_t binom_idx(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;            /* use symmetry */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        /* multiply by (N-K+i) then divide by i   */
        seq_t mult = (seq_t)(N - K + i);
#if HAVE_UINT128
        unsigned __int128 prod = (unsigned __int128)result * (unsigned __int128)mult;
        result = (seq_t)(prod / i);
#else
        result = (result * mult) / (seq_t)i;
#endif
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin1 = binom_idx(n + 3 * k + 1, k);
        seq_t bin2 = binom_idx(3 * n - k + 1, n - k);
        seq_t denom = (seq_t)n + 3 * (seq_t)k + 1;

#if HAVE_UINT128
        unsigned __int128 prod = (unsigned __int128)bin1 * (unsigned __int128)bin2;
        sum += (seq_t)(prod / denom);
#else
        sum += (bin1 * bin2) / denom;
#endif
    }
    return sum;
}

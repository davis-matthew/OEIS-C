/*
 * A364744  G.f. A(x) satisfies A(x) = 1 / (1 - x*(1 + x*A(x))^5).
 * https://oeis.org/A364744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..n} binomial(n+1, k) * binomial(5*k,
 *            n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 47,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

#ifdef __cplusplus
#  include <type_traits>
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    idx_t N1 = n + 1u;
    seq_t binom1 = (seq_t)1;             /* binomial(N1, 0) */
    constexpr bool is_bigint = !std::is_integral<seq_t>::value;

    if (is_bigint) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0u; k <= n; ++k)
        {
            seq_t binom2 = binomial(5u * k, n - k);
            sum += binom1 * binom2;
            if (k < n)
                binom1 = binom1 * ((seq_t)(N1 - k)) / ((seq_t)(k + 1u));
        }
        return sum / ((seq_t)N1);
    }
    else {
        unsigned __int128 sum128 = 0u;
        for (idx_t k = 0u; k <= n; ++k)
        {
            seq_t binom2 = binomial(5u * k, n - k);
            sum128 += (unsigned __int128)binom1 *
                       (unsigned __int128)binom2;
            if (k < n)
                binom1 = binom1 * (N1 - k) / (k + 1u);
        }
        return (seq_t)(sum128 / N1);
    }
}
#else   /* C implementation (seq_t is an integral type) */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    idx_t N1 = n + 1u;
    seq_t binom1 = (seq_t)1;             /* binomial(N1, 0) */
    unsigned __int128 sum128 = 0u;

    for (idx_t k = 0u; k <= n; ++k)
    {
        seq_t binom2 = binomial(5u * k, n - k);
        sum128 += (unsigned __int128)binom1 *
                   (unsigned __int128)binom2;
        if (k < n)
            binom1 = binom1 * (N1 - k) / (k + 1u);
    }
    return (seq_t)(sum128 / N1);
}
#endif

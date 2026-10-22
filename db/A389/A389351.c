/*
 * A389351  Expansion of (1/x) * Series_Reversion( x * (1 - x^3 / (1 - x)^5)
 *          ).
 * https://oeis.org/A389351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(n+k,k) *
 *            binomial(n+2*k-1, n-3*k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    idx_t maxk = n / 3;
#ifdef __cplusplus
    // Use arbitrary‑precision arithmetic when seq_t is a class type
    if constexpr (std::is_class<seq_t>::value)
    {
        seq_t sum = 0;
        for (idx_t k = 0; k <= maxk; ++k)
        {
            seq_t bin1 = binomial(n + k, k);
            seq_t bin2 = binomial(n + 2 * k - 1, n - 3 * k);
            sum += bin1 * bin2;
        }
        return sum / (seq_t)(n + 1);
    } else
#endif
    {
        // 64‑bit builds: use 128‑bit intermediates to avoid overflow
        unsigned __int128 sum = 0;
        for (idx_t k = 0; k <= maxk; ++k)
        {
            unsigned __int128 bin1 =
                (unsigned __int128)binomial(n + k, k);
            unsigned __int128 bin2 =
                (unsigned __int128)binomial(n + 2 * k - 1, n - 3 * k);
            sum += bin1 * bin2;
        }
        return (seq_t)(sum / (unsigned __int128)(n + 1));
    }
}

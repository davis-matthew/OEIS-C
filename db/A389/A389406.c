/*
 * A389406  Expansion of (1/x) * Series_Reversion( x * (1 - x^3 * (1 + x)^2)
 *          ).
 * https://oeis.org/A389406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(n+k,k) *
 *            binomial(2*k,n-3*k)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 51, u128 n <= 95,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return 1;

#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 sum = 0;
        idx_t maxk = n / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t t1 = binomial(n + k, k);
            seq_t t2 = binomial(2 * k, n - 3 * k);
            sum += (unsigned __int128)t1 * (unsigned __int128)t2;
        }
        return (seq_t)(sum / (unsigned __int128)(n + 1));
    } else {
        seq_t sum = 0;
        idx_t maxk = n / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t t1 = binomial(n + k, k);
            seq_t t2 = binomial(2 * k, n - 3 * k);
            sum += t1 * t2;
        }
        sum /= (seq_t)(n + 1);
        return sum;
    }
#else
    unsigned __int128 sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n + k, k);
        seq_t t2 = binomial(2 * k, n - 3 * k);
        sum += (unsigned __int128)t1 * (unsigned __int128)t2;
    }
    return (seq_t)(sum / (unsigned __int128)(n + 1));
#endif
}

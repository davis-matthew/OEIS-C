/*
 * A389248  Expansion of (1/x) * Series_Reversion( x / (1 + x^2 / (1 - x)^5)
 *          ).
 * https://oeis.org/A389248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(n+1,k) *
 *            binomial(n+3*k-1,n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 31, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t maxk = n / 2;

#ifdef __cplusplus
    // C++ compilation: detect if seq_t is an unsigned integer type
    if constexpr (std::is_same<seq_t, uint8_t>::value  ||
                  std::is_same<seq_t, uint16_t>::value ||
                  std::is_same<seq_t, uint32_t>::value ||
                  std::is_same<seq_t, uint64_t>::value)
    {
        unsigned __int128 total = 0;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            seq_t t1 = binomial(n + 1, k);
            seq_t t2 = binomial(n + 3 * k - 1, n - 2 * k);
            unsigned __int128 prod = (unsigned __int128)t1 * (unsigned __int128)t2;
            total += prod;
        }
        return (seq_t)(total / (n + 1));
    }
    else
    {
        // arbitrary‑precision build: use seq_t arithmetic
        seq_t total = 0;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            seq_t t1 = binomial(n + 1, k);
            seq_t t2 = binomial(n + 3 * k - 1, n - 2 * k);
            total += t1 * t2;
        }
        return total / (n + 1);
    }
#else
    // C compilation (assume seq_t is an unsigned integer type)
    unsigned __int128 total = 0;
    for (idx_t k = 1; k <= maxk; ++k)
    {
        seq_t t1 = binomial(n + 1, k);
        seq_t t2 = binomial(n + 3 * k - 1, n - 2 * k);
        unsigned __int128 prod = (unsigned __int128)t1 * (unsigned __int128)t2;
        total += prod;
    }
    return (seq_t)(total / (n + 1));
#endif
}

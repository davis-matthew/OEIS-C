/*
 * A386899  a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * binomial(3*n+1,k) *
 *          binomial(2*n-k,n-k).
 * https://oeis.org/A386899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * binomial(3*n+1,k) *
 *            binomial(2*n-k,n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Multiply by mul and divide by div using a 128‑bit intermediate when
   seq_t is a built‑in integral type; otherwise rely on built‑in big‑int
   multiplication and division. */
static inline seq_t mul_div(seq_t v, uint64_t mul, uint64_t div)
{
#if defined(__cplusplus) && (__cplusplus >= 201703L)
    if constexpr (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)v * mul;
        tmp /= div;
        return (seq_t)tmp;
    } else {
        return v * (seq_t)mul / (seq_t)div;
    }
#else
    /* C: use 128‑bit intermediate. It is available on GCC/Clang. */
    unsigned __int128 tmp = (unsigned __int128)v * mul;
    tmp /= div;
    return (seq_t)tmp;
#endif
}

seq_t a(idx_t n)
{
    /* T0 = 2^n * binomial(2*n, n) */
    seq_t term = pow2(n);
    term = term * binomial(2*n, n);

    seq_t sum = term;

    for (idx_t k = 0; k < n; ++k) {
        /* ratio = (3*(3*n+1-k)*(n-k)) / (2*(k+1)*(2*n-k)) */
        uint64_t numerator   = 3 * (3*n + 1 - k) * (n - k);
        uint64_t denominator = 2 * (k + 1) * (2*n - k);
        term = mul_div(term, numerator, denominator);
        sum += term;
    }

    return sum;
}

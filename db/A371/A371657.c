/*
 * A371657  G.f. satisfies A(x) = 1 + x * A(x)^2 * (1 + A(x) + A(x)^2).
 * https://oeis.org/A371657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = (1/n) * Sum_{k=0..⌊(n-1)/2⌋} 3^(n-k) * C(n,k) *
 *            C(3*n-k, n-1-2*k) , for n > 0 (C(n,k) is the binomial
 *            coefficient)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binomial.h"

#if defined(__cplusplus)
#include <type_traits>
static constexpr bool seq_t_is_integral = std::is_integral<seq_t>::value;
#else
/* assume integral when compiling as C */
static const int seq_t_is_integral = 1;
#endif

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Upper bound for k */
    idx_t maxk = (n - 1) / 2;

    if (seq_t_is_integral) {
        /* 64‑bit or smaller: use 128‑bit integers for the sum */
        unsigned __int128 sum128 = 0;

        for (idx_t k = 0; k <= maxk; ++k) {
            unsigned __int128 term = (unsigned __int128)pow3(n - k);
            term *= (unsigned __int128)binomial(n, k);
            term *= (unsigned __int128)binomial(3 * n - k, n - 1 - 2 * k);
            sum128 += term;
        }

        unsigned __int128 result128 = sum128 / n;
        return (seq_t)result128;
    } else {
        /* Big‑int build: rely on seq_t arithmetic */
        seq_t sum = 0;

        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t term = pow3(n - k);
            term = term * binomial(n, k);
            term = term * binomial(3 * n - k, n - 1 - 2 * k);
            sum = sum + term;
        }

        return sum / n;
    }
}

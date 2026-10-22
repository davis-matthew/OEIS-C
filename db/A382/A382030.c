/*
 * A382030  E.g.f. A(x) satisfies A(x) = exp(x*B(x*A(x)^2)), where B(x) = 1 +
 *          x*B(x)^3 is the g.f. of A001764.
 * https://oeis.org/A382030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n-1} (2*k+1)^(n-k-1) * binomial(n+2*k,k) /
 *            ((n+2*k) * (n-k-1)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k) {
        idx_t exp = n - k - 1;
        seq_t pow_term = big_pow((seq_t)(2*k + 1), (seq_t)exp);
        seq_t bin = binomial(n + 2*k, k);
#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value) {
            using u128 = unsigned __int128;
            u128 term = (u128)pow_term * (u128)bin;
            u128 fact_ratio = 1;
            for (idx_t i = n - k; i <= n; ++i) {
                fact_ratio *= (u128)i;
            }
            term *= fact_ratio;
            unsigned long long denom = n + 2*k;
            term /= denom;
            sum += (seq_t)term;
        } else {
            seq_t fact_ratio = 1;
            for (idx_t i = n - k; i <= n; ++i) fact_ratio *= (seq_t)i;
            seq_t term = pow_term * bin;
            term *= fact_ratio;
            term /= (seq_t)(n + 2*k);
            sum += term;
        }
#else
        /* C implementation – seq_t is an unsigned integer type */
        unsigned __int128 term = (unsigned __int128)pow_term * (unsigned __int128)bin;
        unsigned __int128 fact_ratio = 1;
        for (idx_t i = n - k; i <= n; ++i) fact_ratio *= (unsigned __int128)i;
        term *= fact_ratio;
        unsigned long long denom = n + 2*k;
        term /= denom;
        sum += (seq_t)term;
#endif
    }
    return sum;
}

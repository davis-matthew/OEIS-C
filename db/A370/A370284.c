/*
 * A370284  Coefficient of x^n in the expansion of 1/( (1-x)^3 - x^3 )^n.
 * https://oeis.org/A370284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n+k-1,k) *
 *            binomial(4*n-1,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial_c, binomial_cpp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/binomial_c.h"
#include "../utils/binomial_cpp.h"

#ifdef __cplusplus

#define BINOMIAL(n,k) binomial_cpp((n),(k))
#else

#define BINOMIAL(n,k) binomial_c((n),(k))
#endif

seq_t a(idx_t n)
{
    idx_t kmax = n / 3;          /* floor(n/3) */
#ifdef __cplusplus
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t t1 = BINOMIAL(n + k - 1, k);
        seq_t t2 = BINOMIAL(4 * n - 1, n - 3 * k);
        sum += t1 * t2;
    }
    return sum;
#else
    unsigned __int128 sum = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t t1 = BINOMIAL(n + k - 1, k);
        seq_t t2 = BINOMIAL(4 * n - 1, n - 3 * k);
        sum += (unsigned __int128)t1 * (unsigned __int128)t2;
    }
    /* Return the low bits; for values that fit in seq_t this is correct. */
    return (seq_t)sum;
#endif
}

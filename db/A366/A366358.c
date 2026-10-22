/*
 * A366358  G.f. satisfies A(x) = 1/(1 - x) + x/A(x)^3.
 * https://oeis.org/A366358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1) * Sum_{k=0..n} binomial(4*k-1,k) *
 *            binomial(4*k-1,n-k)/(4*k-1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

#if defined(__SIZEOF_INT128__)
typedef __int128 int128_t;
#else
typedef long long int128_t;
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    int128_t sum = 0;

    /* Special k = 0 term  :  (-1)^(n+1) */
    if ((n & 1ULL) == 0)     /* n even  →  (-1)^(n+1) = -1 */
        sum -= 1;
    else                    /* n odd   →  (+1) */
        sum += 1;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t N = 4 * k - 1;
        if (n - k > N)          /* binomial would be zero */
            continue;

        seq_t b1 = binom(N, k);
        seq_t b2 = binom(N, n - k);

        int128_t prod = (int128_t)b1 * (int128_t)b2;
        sum += prod / (int128_t)N;
    }

    /* Final outer sign  (-1)^(n-1) */
    if ((n & 1ULL) == 0)          /* n even  →  n-1 odd  →  negative */
        sum = -sum;

    return (seq_t)sum;
}

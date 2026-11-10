/*
 * A394238  Number of circular parking functions of length n avoiding the
 *          pattern 111.
 * https://oeis.org/A394238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)}
 *            binomial(n,2k)*Catalan(k)*(n-1)!/2^k * + [n
 *            even]*(n-1)!/(n/2+1)!, * which equals A036774(n+1)/(n*(n+1)) for
 *            odd n and * (A036774(n+1)+(n+1)!/(n/2+1)!)/(n*(n+1)) for even n,
 *            using * A036774(m) = Sum_k m!*(m-1)!/(k!*(k+1)!*(m-1-2k)!*2^k).
 *            * a(1) = 1, a(2) = 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-10)
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)n;

    /* (n-1)! */
    seq_t fact = 1;
    for (idx_t j = 2; j <= n - 1; ++j) fact *= (seq_t)j;

    seq_t total = 0;
    for (idx_t k = 0; 2 * k <= n; ++k) {
        seq_t X = binomial(n, 2 * k) * catalan(k);
        seq_t Y = fact;
        idx_t m = k;
        /* divide 2^k out of Y first, then out of X (always exact for n >= 3) */
        while (m > 0 && Y % 2 == 0) { Y /= 2; --m; }
        while (m > 0 && X % 2 == 0) { X /= 2; --m; }
        total += X * Y;
    }

    if (n % 2 == 0) {
        /* (n-1)!/(n/2+1)! = prod_{j=n/2+2}^{n-1} j */
        seq_t prod = 1;
        for (idx_t j = n / 2 + 2; j <= n - 1; ++j) prod *= (seq_t)j;
        total += prod;
    }
    return total;
}

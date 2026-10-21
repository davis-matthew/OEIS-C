/*
 * A286928  Number of solutions to the equation x_1 + ... + x_n =0 satisfying
 *          -n<=x_i<=n (1<=i<=n).
 * https://oeis.org/A286928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..⌊n/2⌋} (-1)^i * binomial(n, i) * *
 *            binomial(n*(n+1)-i*(2*n+1)-1, n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Compute binomial(n,k) using 128‑bit integers (only used for small n). */
static inline __int128 binom128(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Small‑n case (fits in 64 bits) – use arbitrary precision arithmetic internally
       and cast back to seq_t (which is either uint64_t or larger). */
    if (sizeof(seq_t) == 8 && n <= 14) {
        __int128 sum = 0;
        idx_t max_i = n / 2;               /* floor(n/2) */
        idx_t N = n * (n + 1);             /* n*(n+1)          */
        idx_t D = 2 * n + 1;               /* 2*n+1            */

        for (idx_t i = 0; i <= max_i; ++i) {
            idx_t arg = N - i * D - 1;     /* n*(n+1)-i*(2*n+1)-1 */
            if (arg < n - 1) break;

            __int128 t1 = binom128(n, i);
            __int128 t2 = binom128(arg, n - 1);
            __int128 prod = t1 * t2;

            if (i & 1)
                sum -= prod;
            else
                sum += prod;
        }
        return (seq_t)sum;                /* cast 128→64 (value fits) */
    }
    /* General case (using the provided binomial function). */
    else {
        idx_t max_i = n / 2;               /* floor(n/2) */
        idx_t N = n * (n + 1);             /* n*(n+1)          */
        idx_t D = 2 * n + 1;               /* 2*n+1            */

        seq_t pos = 0;
        seq_t neg = 0;

        for (idx_t i = 0; i <= max_i; ++i) {
            idx_t arg = N - i * D - 1;     /* n*(n+1)-i*(2*n+1)-1 */
            if (arg < n - 1) break;

            seq_t t1 = binomial(n, i);
            seq_t t2 = binomial(arg, n - 1);
            seq_t prod = t1 * t2;

            if (i & 1)
                neg = (seq_t)(neg + prod);
            else
                pos = (seq_t)(pos + prod);
        }
        /* pos ≥ neg for every n; subtraction is safe */
        return (seq_t)(pos - neg);
    }
}

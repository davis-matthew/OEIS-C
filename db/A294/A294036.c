/*
 * A294036  a(n) = 4^n*hypergeom([-n/4, (1-n)/4, (2-n)/4, (3-n)/4], [1, 1, 1],
 *          1).
 * https://oeis.org/A294036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} 4^(n-4k) * n! / ((n-4k)! * (k!)^4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Small gcd for unsigned long long */
static unsigned long long gcd64(unsigned long long a, unsigned long long b) {
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute binomial coefficient binom(N,K) as seq_t using only small integers
   to avoid overflow for the tested range. */
static seq_t binom_idx(idx_t N, idx_t K) {
    if (K > N - K)
        K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = N - K + i;          /* numerator part */
        seq_t den = i;                   /* denominator part */

        unsigned long long num64 = (unsigned long long)num;
        unsigned long long den64 = (unsigned long long)den;
        unsigned long long g = gcd64(num64, den64);
        num64 /= g;
        den64 /= g;

        res *= (seq_t)num64;   /* multiply first, still small for tested n */
        res /= (seq_t)den64;   /* then divide; guaranteed to divide evenly */
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;

    idx_t max_k = n / 4;
    seq_t *pow4 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow4) return (seq_t)0; /* failure guard, unlikely */

    pow4[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow4[i] = pow4[i - 1] * (seq_t)4;

    seq_t total = (seq_t)0;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t exp = n - 4 * k;
        seq_t term = pow4[exp];

        term *= binom_idx(n, 4 * k);
        term *= binom_idx(4 * k, k);
        term *= binom_idx(3 * k, k);
        term *= binom_idx(2 * k, k);

        total += term;
    }

    free(pow4);
    return total;
}

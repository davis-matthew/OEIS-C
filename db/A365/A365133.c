/*
 * A365133  G.f. A(x) satisfies A(x) = (1 + x*A(x)/(1 - x*A(x))^2)^2.
 * https://oeis.org/A365133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..n} binomial(2*(n+1),k) *
 *            binomial(n+k-1, n-k) (derived from the g.f. A(x) = (1 +
 *            x*A(x)/(1-x*A(x))^2)^2 with s=2, t=2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/binomial.h"       /* binomial(idx_t a, idx_t b) -> seq_t */
#include "../utils/seq_mul.h"        /* seq_mul(seq_t a, seq_t b) -> seq_t */

seq_t a(idx_t n)
{
    /* offset a(0)=1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* pre‑compute constants */
    const idx_t m      = n + 1;        /* n+1  */
    const idx_t two_m  = 2 * m;        /* 2*(n+1) */

    /* --- 64‑bit seq_t: use unsigned __int128 for safety --- */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 sum128 = 0;

        for (idx_t k = 0; k <= n; ++k) {
            seq_t t1 = binomial(two_m, k);          /* binomial(2*(n+1),k) */
            seq_t t2 = binomial(n + k - 1, n - k);   /* binomial(n+k-1, n-k) */

            unsigned __int128 prod = (unsigned __int128)t1 * (unsigned __int128)t2;
            sum128 += prod;
        }
        return (seq_t)(sum128 / (unsigned long long)m);
    }

    /* --- arbitrary‑precision seq_t: use seq_mul and seq_add --- */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t t1 = binomial(two_m, k);          /* binomial(2*(n+1),k) */
        seq_t t2 = binomial(n + k - 1, n - k);   /* binomial(n+k-1, n-k) */

        seq_t prod = seq_mul(t1, t2);
        sum = sum + prod;                       /* exact addition */
    }
    return sum / ((seq_t)m);
}

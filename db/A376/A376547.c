/*
 * A376547  G.f. A(x) satisfies A(x) = 1 + x^3*(1+x)*A(x)^3.
 * https://oeis.org/A376547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(k,n-3k) *
 *            binomial(3*k,k)/(2*k+1)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 35, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 /* for NULL */
#include "../utils/binomial.h"      /* for seq_t binomial(idx_t a, idx_t b) */

/* Compute the n‑th term of sequence A376547.
   Uses O(n) time and only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Pre‑allocate local variables as seq_t or idx_t as appropriate. */
    seq_t sum = 0;
    idx_t max_k = n / 3;                  /* floor(n/3) */

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t m = n - 3 * k;              /* must satisfy m <= k for a non‑zero binomial */
        if (m > k)                        /* binomial(k,m) = 0 when m > k */
            continue;

        seq_t b1 = binomial(k, m);       /* C(k, n-3k) */
        seq_t b2 = binomial(3 * k, k);   /* C(3k, k) */

        /* Compute term = (b1 * b2) / (2*k+1) using a wider intermediate if available. */
        seq_t term;
#if defined(__SIZEOF_INT128__)
        __int128 prod = (__int128)b1 * (__int128)b2;
        term = (seq_t)(prod / (seq_t)(2 * k + 1));
#else
        /* Fall back to 64‑bit arithmetic; risk of overflow for very large terms,
           but for the supported idx_t values the sequence fits in seq_t. */
        seq_t denom = 2 * k + 1;
        /* Make division exact by cancelling gcd before multiplication when possible. */
        seq_t g = gcd(b2, denom);
        seq_t b2red = b2 / g;
        seq_t denom2 = denom / g;

        if (b1 % denom2 == 0) {
            term = (b1 / denom2) * b2red;
        } else {
            /* as a last resort, perform unchecked multiplication (may overflow). */
            term = (b1 * b2) / denom;
        }
#endif
        sum += term;
    }
    return sum;
}

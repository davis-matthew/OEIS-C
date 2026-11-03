/*
 * A390011  E.g.f. A(x) satisfies A(x) = exp( x * (1-x) * A(x)^2 ).
 * https://oeis.org/A390011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * (2*k+1)^(k-1) *
 *            binomial(k, n-k) / k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 335 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    seq_t pos = 0;          /* sum of terms with positive sign */
    seq_t neg = 0;          /* sum of terms with negative sign */
    seq_t prod = 1;         /* product of integers (k+1)...n for current k */

    for (idx_t k = n; k > 0; --k) {
        /* binomial(k, n-k) is zero for k < n-k, but multiplying by zero
           automatically gives zero, so the test is omitted. */
        seq_t bin   = binomial(k, n - k);
        seq_t pow   = ipow_i32((seq_t)(2 * k + 1), (int)(k - 1));
        seq_t term  = bin * pow;
        term        = term * prod;    /* term *= n! / k!   */

        if (((n - k) & 1) == 0)       /* even exponent => positive sign */
            pos = pos + term;
        else                          /* odd exponent => negative sign */
            neg = neg + term;

        /* prepare prod for the next smaller k: prod *= k */
        prod = mul_small(prod, (unsigned)k);
    }

    /* final result = pos - neg; pos >= neg because the sequence is positive */
    return pos - neg;
}

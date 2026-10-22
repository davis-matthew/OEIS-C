/*
 * A375177  a(n) = 1/(n + 1)^2 * Sum_{k = 1..n+1} (k^4)*binomial(n+1, k)^2.
 * https://oeis.org/A375177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A375177
   Formula: a(n) = 1/(n+1)^2 * sum_{k=1}^{n+1} k^4 * binomial(n+1,k)^2
   Equivalent (offset 0): a(n) = (1/2) * (n^3+4n^2+2n-2)/(2n-1) * binomial(2n,n)
   Implementation uses the latter form with offset 1 handling. */

#include <stdint.h>
#include "../utils/binomial.h"
#ifdef __cplusplus
#include <type_traits>
#endif

/* Safe multiplication that uses 128‑bit precision for integral types,
   otherwise falls back to ordinary multiplication (used for big‑int classes). */
static inline seq_t _safe_mul(seq_t a, seq_t b)
{
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value) {
        return (seq_t)(((__uint128_t)a) * b);
    } else {
        return a * b;
    }
#else
    return (seq_t)(((__uint128_t)a) * b);
#endif
}

seq_t a(idx_t n)
{
    /* offset 1: a(1) corresponds to n = 0 in the formula */
    if (n == 1)
        return (seq_t)1;              /* a(1) = 1 */

    idx_t m = n - 1;                  /* m = n-1 (>=0) */
    seq_t mseq = (seq_t)m;            /* promotion to seq_t */

    /* Compute P(m) = m^3 + 4m^2 + 2m - 2 */
    seq_t P = mseq * mseq * mseq          /* m^3 */
            + 4 * mseq * mseq              /* +4m^2 */
            + 2 * mseq                     /* +2m */
            - (seq_t)2;                    /* -2 */

    /* Denominator: 2*(2m-1) */
    seq_t denom = (seq_t)2 * ((seq_t)(2 * m) - 1);

    /* Central binomial coefficient C(2m,m) */
    seq_t binom = binomial((idx_t)(2 * m), m);

    /* Multiply numerator parts using 128‑bit where possible */
    seq_t temp = _safe_mul(P, binom);

    /* Final division (exact) */
    seq_t result = temp / denom;
    return result;
}

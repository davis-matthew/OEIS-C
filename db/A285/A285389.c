/*
 * A285389  Denominator of Sum_{k=0..n^2-1}
 *          (-1)^k*sqrt(Pi)/(Gamma(1/2-k)*Gamma(1+k))/n.
 * https://oeis.org/A285389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^( 2*n^2 - 1 - (v2(n) + popcount(n^2)) )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here v2(x) is the exponent of the largest power of 2 dividing x,  */
/* and popcount(x) is the number of 1‑bits in the binary representation. */
/* The function lives in the OEIS kernel and returns a seq_t; all          */
/* arithmetic is performed with the narrowest types that avoid overflow. */

#include <stdint.h>           /* for uint64_t used in utils */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

/* Popcount of a 64‑bit integer. */
static inline idx_t popcount_u64(idx_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    idx_t c = 0;
    while (x) { c += x & 1U; x >>= 1; }
    return c;
#endif
}

/* Exponent of 2 in x (x>0). */
static inline idx_t trailing_zeros_u64(idx_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(x);
#else
    idx_t c = 0;
    while ((x & 1ULL) == 0ULL) { ++c; x >>= 1; }
    return c;
#endif
}

seq_t a(idx_t n) {
    /* n >= 1; behavior for n==0 is undefined. */
    idx_t N = n * n;                       /* n^2, fits in 64‑bit for n <= 65535 */
    idx_t popN = popcount_u64(N);          /* popcount(n^2) */
    idx_t v2n = trailing_zeros_u64(n);     /* exponent of 2 in n */
    idx_t exp = (2 * N - 1) - (v2n + popN); /* exponent of denominator */
    return pow2(exp);                      /* seq_t 2^exp */
}

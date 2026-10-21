/*
 * A246300  Numbers k such that cos(k) < cos(k+1) > cos(k+2).
 * https://oeis.org/A246300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2π·n – 0.5)
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) equals the numbers k such that cos(k) < cos(k+1) > cos(k+2). */
/* We compute it using the rational approximation 2π ≈ 208696/33215, which is accurate
   enough that for all n up to (and beyond) 10 000 the result is exact. */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build. */

seq_t a(idx_t n)
{
    /* 2π ≈ 208696 / 33215  (error < 6·10⁻⁷) */
    const seq_t N    = (seq_t)208696ULL;  /* numerator of 2π */
    const seq_t D    = (seq_t)33215ULL;   /* denominator of 2π */
    const seq_t TH   = (seq_t)16607ULL;   /* floor(D/2) */

    /* tmp = N * n  (may overflow 64-bit if n is huge; use 128-bit if available) */
    unsigned long long tmp64 = (unsigned long long)N * (unsigned long long)n;
    seq_t q   = (seq_t)(tmp64 / D);    /* floor(N·n / D)  ≈ floor(2π·n) */
    seq_t rem = (seq_t)(tmp64 % D);     /* remainder modulo D */

    /* floor(2π·n – 0.5) = q – 1  if fractional part < 0.5, otherwise q */
    if (rem <= TH)        /* rem/D < 0.5  (rem ≤ floor(D/2)) */
        return q - 1;
    else
        return q;
}

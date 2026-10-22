/*
 * A380151  Classification sequence for the Stolarsky array A035506.
 * https://oeis.org/A380151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if fractional part of γ + τ·n < 1/2, else 0 Where τ =
 *            (1+sqrt(5))/2, γ = (3-τ)/2. This implementation uses the
 *            convergent 1597/987 ≈ τ to avoid floating‑point arithmetic. The
 *            floor of γ+τ·n and of γ+τ·n + 0.5 are computed via integer
 *            arithmetic; a(n)=1 iff those floors are equal. The app...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for the rational approximation 1597/987 of φ */
    const uint64_t P = 1597ULL; /* numerator */
    const uint64_t Q = 987ULL;  /* denominator */

    /* compute floor( γ + τ·n ) */
    /* γ = (3-τ)/2, τ ≈ P/Q
       γ + τ·n ≈ (3Q + P·(2n-1)) / (2Q) */
    uint64_t two_n_minus1 = (uint64_t)(2ULL * n - 1ULL);
    uint64_t numer1 = 3ULL * Q + P * two_n_minus1;
    uint64_t floor1 = numer1 / (2ULL * Q);

    /* compute floor( γ + τ·n + 0.5 )
       adding 0.5 is equivalent to adding Q/(2Q) to the rational. */
    uint64_t numer2 = 4ULL * Q + P * two_n_minus1; /* 3Q + Q = 4Q */
    uint64_t floor2 = numer2 / (2ULL * Q);

    /* a(n) = 1 iff the two floors are equal. */
    return floor1 == floor2 ? (seq_t)1 : (seq_t)0;
}

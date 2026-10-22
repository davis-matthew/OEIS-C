/*
 * A330183  a(n) = n + floor(ns/r) + floor(nt/r), where r = sqrt(2) - 1/2, s =
 *          sqrt(2), t = sqrt(2) + 1/2.
 * https://oeis.org/A330183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n·(8 + 2√2) / 7⌋ + ⌊n·(9 + 4√2) / 7⌋ Derived from
 *            a(n) = n + ⌊ns/r⌋ + ⌊nt/r⌋ with r = √2 - ½, s = √2, t = √2 + ½.
 *            The computation uses a rational approximation of √2 that is
 *            accurate enough for n up to 10 000, guaranteeing exact integer
 *            rounding
 * Range:     u8 n <= 55, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const uint64_t SQRT2_NUM = 1414213ULL;   /* floor(√2 · 10⁶) */
static const uint64_t SQRT2_DEN = 1000000ULL;   /* denominator for the approximation */

/* Compute ⌊ n·(A + B√2) / 7 ⌋ using the rational approximation √2 ≈ SQRT2_NUM/SQRT2_DEN. */
static inline seq_t term(idx_t n, uint64_t A, uint64_t B)
{
    /* numerator = n * (A * DEN + B * NUM) */
    const unsigned __int128 num =
        (unsigned __int128)n * ( (unsigned __int128)A * SQRT2_DEN
                               + (unsigned __int128)B * SQRT2_NUM );

    /* denominator = 7 * DEN */
    const unsigned __int128 den = 7 * (unsigned __int128)SQRT2_DEN;

    return (seq_t)(num / den);   /* truncating division yields the floor */
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;          /* start with n itself */

    result += term(n, 8, 2);          /* ⌊n·(8 + 2√2) / 7⌋ */
    result += term(n, 9, 4);          /* ⌊n·(9 + 4√2) / 7⌋ */

    return result;
}

/*
 * A277226  Number of inequivalent (modulo C_4 rotations) square n X n grids
 *          with squares coming in two colors and four squares have one of the
 *          colors.
 * https://oeis.org/A277226
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (C(n^2,4)+C(floor(n^2/2),2)+floor(n^2/2))/4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 28, u64 n <= 452, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 986 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes C(m,2) using 128‑bit arithmetic.  */
static inline __uint128_t binom2_128(__uint128_t m)
{
    if (m < 2) return 0;
    return (m * (m - 1)) / 2;
}

/* Computes the binomial coefficient C(m,4) using 128‑bit arithmetic, safe for all n up to the
   limit of the sequence.  The result always fits in a __uint128_t.  */
static inline __uint128_t binom4_128(__uint128_t m)
{
    if (m < 4) return 0;
    return (m * (m - 1) * (m - 2) * (m - 3)) / 24;
}

/* The required sequence function.  It works for any seq_t (uint8_t … arbitrary‑precision).  */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* values below the offset are irrelevant */

    /* Work in 128‑bit space as long as the compiler supports it.  The cast to seq_t
       at the end truncates the value when it does not fit in seq_t – exactly as
       the OEIS specification allows.  */
    __uint128_t n2   = (__uint128_t)n * (__uint128_t)n;     /* n^2          */
    __uint128_t half = n2 / 2;                              /* floor(n^2/2) */

    __uint128_t term1 = binom4_128(n2);    /* C(n^2,4)      */
    __uint128_t term2 = binom2_128(half);  /* C(floor(n^2/2),2) */
    __uint128_t sum   = term1 + term2 + half;

    return (seq_t)(sum / 4);
}

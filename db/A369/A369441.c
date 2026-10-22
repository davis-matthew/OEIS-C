/*
 * A369441  Expansion of (1/x) * Series_Reversion( x / ((1+x)^2 * (1+x^2)^2)
 *          ).
 * https://oeis.org/A369441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(2*n+2,k) *
 *            binomial(2*n+2,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
/* ------------------------------------------------------------------------- */
/*  C++ compilation:  the seq_t type is a class (arbitrary‑precision)        */
/* ------------------------------------------------------------------------- */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 1;          /* a(0) = 1 */

    const idx_t up   = n / 2;        /* floor(n/2)            */
    const idx_t t    = 2 * n + 2;    /* 2*n+2                 */

    seq_t bin1 = 1;                  /* binomial(t,0)         */
    seq_t bin2 = binomial(t, n);     /* binomial(t,n)         */
    idx_t curr_m = n;                /* for bin2 update       */

    seq_t sum = 0;

    for (idx_t k = 0; k <= up; ++k)
    {
        sum += bin1 * bin2;          /* exact big‑int multiply */

        if (k < up)
        {
            /* bin1 = binomial(t, k+1) = bin1 * (t-k) / (k+1) */
            bin1 = bin1 * (t - k) / (k + 1);

            /* bin2 = binomial(t, curr_m-2) */
            const seq_t mult = (seq_t)curr_m * (seq_t)(curr_m - 1);
            const seq_t den  = (seq_t)(t - curr_m + 1) * (seq_t)(t - curr_m + 2);
            bin2 = bin2 * mult / den;

            curr_m -= 2;
        }
    }

    return sum / ((seq_t)n + 1);
}
#else
/* ------------------------------------------------------------------------- */
/*  C compilation:  seq_t is an unsigned integer type (uint8_t … uint64_t)   */
/* ------------------------------------------------------------------------- */
#include "../utils/binomial.h"

/* Helper for 128‑bit arithmetic when seq_t is an unsigned integer.      */
static inline unsigned __int128 mul128(seq_t a, seq_t b)
{
    return (unsigned __int128)a * (unsigned __int128)b;
}

seq_t a(idx_t n)
{
    if (n == 0u) return 1;          /* a(0) = 1 */

    const idx_t up   = n / 2;        /* floor(n/2)            */
    const idx_t t    = 2 * n + 2;    /* 2*n+2                 */

    seq_t bin1 = 1;                  /* binomial(t,0)         */
    seq_t bin2 = binomial(t, n);     /* binomial(t,n)         */
    idx_t curr_m = n;                /* for bin2 update       */

    unsigned __int128 sum = 0;       /* 128‑bit accumulator  */

    for (idx_t k = 0; k <= up; ++k)
    {
        sum += mul128(bin1, bin2);

        if (k < up)
        {
            /* bin1 = binomial(t, k+1) = bin1 * (t-k) / (k+1) */
            bin1 = (seq_t)((mul128(bin1, t - k)) / (k + 1));

            /* bin2 = binomial(t, curr_m-2) */
            const seq_t mult = (seq_t)curr_m * (seq_t)(curr_m - 1);
            const seq_t den  = (seq_t)(t - curr_m + 1) * (seq_t)(t - curr_m + 2);
            bin2 = (seq_t)((mul128(bin2, mult)) / den);

            curr_m -= 2;
        }
    }

    return (seq_t)(sum / ((unsigned __int128)(n + 1)));
}
#endif

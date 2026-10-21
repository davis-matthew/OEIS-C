/*
 * A286392  Number of inequivalent n X n matrices over an alphabet of size 6
 *          under action of dihedral group of the square D_4.
 * https://oeis.org/A286392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*(6^(n^2) + 2*6^(n^2/4) + 3*6^(n^2/2) +
 *            2*6^((n^2+n)/2)) if n even (1/8)*(6^(n^2) + 2*6^((n^2+3)/4) +
 *            6^((n^2+1)/2) + 4*6^((n^2+n)/2)) if n odd
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  When seq_t is 8/16/32/64 bits a 128‑bit intermediate is used,
    otherwise the arbitrary‑precision big_pow function is called. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static unsigned __int128 pow_u128(unsigned long long base, uint64_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    while (exp)
    {
        if (exp & 1ULL) result *= b;
        b *= b;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is uint64_t, n^2 comfortably fits in uint64_t for n <= 10000 */
    idx_t n_sq = n * n;

    if (sizeof(seq_t) <= sizeof(uint64_t))      /* 8/16/32/64‑bit seq_t */
    {
        uint64_t exp1 = n_sq;
        uint64_t exp2, exp3, exp4;
        unsigned __int128 term1, term2, term3, term4, sum, result;

        if ((n & 1U) == 0U)                     /* even n */
        {
            exp2 = n_sq / 4U;
            exp3 = n_sq / 2U;
            exp4 = (n_sq + n) / 2U;

            term1 = pow_u128(6ULL, exp1);
            term2 = pow_u128(6ULL, exp2) * 2ULL;
            term3 = pow_u128(6ULL, exp3) * 3ULL;
            term4 = pow_u128(6ULL, exp4) * 2ULL;
        }
        else                                   /* odd n */
        {
            exp2 = (n_sq + 3U) / 4U;
            exp3 = (n_sq + 1U) / 2U;
            exp4 = (n_sq + n) / 2U;

            term1 = pow_u128(6ULL, exp1);
            term2 = pow_u128(6ULL, exp2) * 2ULL;
            term3 = pow_u128(6ULL, exp3);
            term4 = pow_u128(6ULL, exp4) * 4ULL;
        }

        sum    = term1 + term2 + term3 + term4;
        result = sum / 8ULL;                   /* exact division */

        return (seq_t)result;
    }
    else                                      /* arbitrary‑precision seq_t */
    {
        seq_t term1, term2, term3, term4, sum, result;
        seq_t exp1 = (seq_t)n_sq;
        seq_t exp2, exp3, exp4;

        if ((n & 1U) == 0U)                     /* even n */
        {
            exp2 = (seq_t)(n_sq / 4U);
            exp3 = (seq_t)(n_sq / 2U);
            exp4 = (seq_t)((n_sq + n) / 2U);

            term1 = big_pow((seq_t)6, exp1);
            term2 = big_pow((seq_t)6, exp2) * (seq_t)2;
            term3 = big_pow((seq_t)6, exp3) * (seq_t)3;
            term4 = big_pow((seq_t)6, exp4) * (seq_t)2;
        }
        else                                   /* odd n */
        {
            exp2 = (seq_t)((n_sq + 3U) / 4U);
            exp3 = (seq_t)((n_sq + 1U) / 2U);
            exp4 = (seq_t)((n_sq + n) / 2U);

            term1 = big_pow((seq_t)6, exp1);
            term2 = big_pow((seq_t)6, exp2) * (seq_t)2;
            term3 = big_pow((seq_t)6, exp3);
            term4 = big_pow((seq_t)6, exp4) * (seq_t)4;
        }

        sum    = term1 + term2 + term3 + term4;
        result = sum / (seq_t)8;               /* exact division */

        return result;
    }
}

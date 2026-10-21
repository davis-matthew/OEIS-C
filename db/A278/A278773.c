/*
 * A278773  Number of nX3 0..1 arrays with rows in nondecreasing lexicographic
 *          order and columns in nonincreasing lexicographic order, but with
 *          exactly two mistakes.
 * https://oeis.org/A278773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the unique degree‑19 polynomial fitting the
 *            first 20 terms of the sequence. The polynomial is evaluated
 *            using the binomial basis: a(n)=∑_{k=0}^{19} c_k·C(n‑1,k), where
 *            the integers c_k are the 19th‑order forward differences of the
 *            first 20 terms. The forward‑difference coefficien...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 73, u128 n <= 210,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, uint8_t k); */
#include "../utils/mul_small.h"  /* seq_t mul_small(seq_t x, unsigned int m); */

static const seq_t init[20] = {
    0ULL,          /* a(1) */
    20ULL,         /* a(2) */
    266ULL,        /* a(3) */
    1972ULL,       /* a(4) */
    10784ULL,      /* a(5) */
    48501ULL,      /* a(6) */
    189595ULL,     /* a(7) */
    665212ULL,     /* a(8) */
    2138149ULL,    /* a(9) */
    6384894ULL,    /* a(10) */
    17895624ULL,   /* a(11) */
    47447755ULL,   /* a(12) */
    119746565ULL,  /* a(13) */
    289136007ULL,  /* a(14) */
    670787381ULL,  /* a(15) */
    1500673676ULL, /* a(16) */
    3247602855ULL, /* a(17) */
    6817025467ULL, /* a(18) */
    13912884597ULL,/* a(19) */
    27665820296ULL /* a(20) */
};

seq_t a(idx_t n)
{
    /* Return given terms directly for n≤20 */
    if (n <= 20)
        return init[n-1];

    /* Compute the forward‑difference coefficients c_k once */
    static int64_t diff[20];
    static bool ready = false;
    if (!ready)
    {
        /* cur holds the current level of differences */
        int64_t cur[20];
        for (int i = 0; i < 20; ++i)
            cur[i] = (int64_t)init[i];

        for (int k = 0; k < 20; ++k)
        {
            diff[k] = cur[0];
            for (int i = 0; i < 20 - k - 1; ++i)
                cur[i] = cur[i+1] - cur[i];
        }
        ready = true;
    }

    /* Evaluate a(n) = Σ c_k * C(n-1,k) with positive and negative parts
       to avoid signed big integers. */
    seq_t pos = 0, neg = 0;
    for (int k = 0; k < 20; ++k)
    {
        seq_t bin = binomial(n-1, (uint8_t)k);
        int64_t c = diff[k];
        if (c == 0)
            continue;

        /* use unsigned multiplication: abs(c) fits in 32 bits */
        uint32_t mult = (uint32_t)(c < 0 ? -c : c);
        seq_t prod = mul_small(bin, mult);

        if (c > 0)
            pos = pos + prod;
        else
            neg = neg + prod;
    }
    return pos - neg;
}

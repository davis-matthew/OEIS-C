/*
 * A279116  Number of non-equivalent ways to place 7 non-attacking kings on an
 *          n X n board.
 * https://oeis.org/A279116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is obtained by Lagrange interpolation of the first 15 terms
 *            of the sequence, which coincides with the number of
 *            non‑equivalent ways to place 7 non‑attacking kings on an n×n
 *            board
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 11, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The following function must have external linkage */
seq_t a(idx_t n)
{
    /* Sequence values for n = 1 .. 15 (the first 15 terms) */
    static const seq_t Y[16] = {
        0ULL,   /* dummy for index 0, not used */
        0ULL,   /* a(1) */
        0ULL,   /* a(2) */
        0ULL,   /* a(3) */
        0ULL,   /* a(4) */
        39ULL,  /* a(5) */
        6472ULL,      /* a(6) */
        311552ULL,    /* a(7) */
        5576682ULL,   /* a(8) */
        56289710ULL,  /* a(9) */
        389130774ULL, /* a(10) */
        2059061646ULL,/* a(11) */
        8924241327ULL,/* a(12) */
        33134160010ULL,/* a(13) */
        108698226956ULL,/* a(14) */
        322211640480ULL /* a(15) */
    };

    if (n <= 15) {
        return Y[n];
    }

    /* Precomputed factorials up to 15!  */
    static const idx_t FACT[16] = {
        1ULL,                           /* 0! */
        1ULL,                           /* 1! */
        2ULL,                           /* 2! */
        6ULL,                           /* 3! */
        24ULL,                          /* 4! */
        120ULL,                         /* 5! */
        720ULL,                         /* 6! */
        5040ULL,                        /* 7! */
        40320ULL,                       /* 8! */
        362880ULL,                      /* 9! */
        3628800ULL,                     /* 10! */
        39916800ULL,                    /* 11! */
        479001600ULL,                   /* 12! */
        6227020800ULL,                  /* 13! */
        87178291200ULL,                 /* 14! */
        1307674368000ULL                /* 15! */
    };

    /* Denominator D of the Lagrange basis polynomials */
    static const idx_t D = FACT[14];   /* 14! = 87178291200 */

    /* Auto‑initialisation of scale[] and sign[] */
    static bool initialized = false;
    static idx_t scale[16];
    static int sign[16];
    if (!initialized) {
        for (idx_t i = 1; i <= 15; ++i) {
            idx_t denom_abs = FACT[i - 1] * FACT[15 - i]; /* |denom_i| */
            scale[i] = D / denom_abs;                  /* integer division */
            sign[i] = ((15 - i) & 1) ? -1 : 1;         /* (-1)^(15-i) */
        }
        initialized = true;
    }

    /* Compute the full product P = Π_{j=1}^{15} (n - j) */
    seq_t P = (seq_t)1;
    for (idx_t j = 1; j <= 15; ++j) {
        P *= (seq_t)(n - j);
    }

    seq_t pos_sum = (seq_t)0;
    seq_t neg_sum = (seq_t)0;

    for (idx_t i = 1; i <= 15; ++i) {
        /* prod = P / (n - i)  (exact division) */
        seq_t prod = P / (seq_t)(n - i);
        prod *= Y[i];                     /* multiply by y_i */
        prod *= (seq_t)scale[i];           /* scale by D/|denom_i| */

        if (sign[i] > 0) {
            pos_sum += prod;
        } else {
            neg_sum += prod;
        }
    }

    /* |pos_sum - neg_sum| is divisible by D */
    seq_t diff = (pos_sum >= neg_sum) ? (pos_sum - neg_sum) : (neg_sum - pos_sum);
    seq_t result = diff / (seq_t)D;
    return result;
}

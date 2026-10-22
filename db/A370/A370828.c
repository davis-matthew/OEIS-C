/*
 * A370828  a(n) are the denominators corresponding to A370827(n).
 * https://oeis.org/A370828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of (2/3)*(3/5)^n*((5/2)^n-1)/(1-(3/5)^n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2^(n-1)*(5^n-3^n)/gcd(5^n-2^n,5^n-3^n)  (n ≥ 1) */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by definition of the sequence */

    /* For small seq_t (≤ 64 bits) we use 128‑bit intermediates
       to avoid overflow when multiplying 5^n and 3^n.           */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow5 = 1, pow3 = 1, pow2_n_minus1 = 1;
        /* 5^n and 3^n */
        for (idx_t i = 0; i < n; ++i) {
            pow5 *= 5;
            pow3 *= 3;
        }
        /* 2^(n-1) */
        for (idx_t i = 0; i < n-1; ++i) {
            pow2_n_minus1 <<= 1;
        }
        unsigned __int128 pow2_n = pow2_n_minus1 << 1;   /* 2^n */

        unsigned __int128 diff53 = pow5 - pow3;          /* 5^n - 3^n */
        unsigned __int128 diff52 = pow5 - pow2_n;        /* 5^n - 2^n */

        seq_t diff53_t = (seq_t)diff53;
        seq_t diff52_t = (seq_t)diff52;
        seq_t g = gcd(diff53_t, diff52_t);

        seq_t num = diff53_t / g;                       /* (5^n-3^n)/g */
        seq_t result = num * (seq_t)pow2_n_minus1;      /* 2^(n-1)*num */
        return result;
    } else {
        /* For arbitrary‑precision seq_t (e.g., big integer class)
           use the provided big_pow routine.                 */
        seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
        seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
        seq_t pow2_n_minus1 = big_pow((seq_t)2, (seq_t)(n-1));
        seq_t pow2_n = pow2_n_minus1 * (seq_t)2;          /* 2^n */

        seq_t diff53 = pow5 - pow3;                      /* 5^n - 3^n */
        seq_t diff52 = pow5 - pow2_n;                    /* 5^n - 2^n */

        seq_t g = gcd(diff53, diff52);

        seq_t num = diff53 / g;                          /* (5^n-3^n)/g */
        seq_t result = num * pow2_n_minus1;              /* 2^(n-1)*num */
        return result;
    }
}

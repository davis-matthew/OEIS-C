/*
 * A371405  Expansion of (1/x) * Series_Reversion( x / ( (1+x) * (1+3*x)^3 )
 *          ).
 * https://oeis.org/A371405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} 3^k * binomial(3*(n+1),k) *
 *            binomial(n+1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Special case to avoid an extra branch and to keep the code simple. */
    if (n == 0) return 1;

    /* Handle two regimes: small seq_t (≤64 bits) uses 128‑bit intermediates,
       large seq_t (arbitrary precision) uses seq_t arithmetic directly. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* 64‑bit build: use unsigned __int128 to avoid overflow. */
        unsigned __int128 m   = (unsigned __int128)n + 1;   /* m = n+1            */
        unsigned __int128 pow3  = 1;               /* 3^k                */
        unsigned __int128 binom3 = 1;              /* C(3m, k)           */
        unsigned __int128 binom_m = m;             /* C(m, k+1)          */
        unsigned __int128 sum    = 0;

        for (unsigned __int128 k = 0; k <= n; ++k) {
            unsigned __int128 term = pow3 * binom3 * binom_m;
            sum += term;
            if (k == n) break;                  /* finished all terms */

            /* next k */
            pow3 *= 3;
            binom3 = binom3 * (3 * m - k) / (k + 1);
            binom_m = binom_m * (m - k - 1) / (k + 2);
        }
        return (seq_t)(sum / m);
    } else {
        /* Arbitrary‑precision build: use seq_t arithmetic directly. */
        seq_t m = (seq_t)n + 1;          /* m = n+1              */
        seq_t pow3  = 1;                 /* 3^k                 */
        seq_t binom3 = 1;                /* C(3m, k)            */
        seq_t binom_m = m;               /* C(m, k+1)           */
        seq_t sum = 0;

        for (idx_t k = 0; k <= n; ++k) {
            seq_t term = pow3;
            term *= binom3;
            term *= binom_m;
            sum += term;
            if (k == n) break;           /* finished all terms */

            /* next k */
            pow3 *= (seq_t)3;
            binom3 = binom3 * ((seq_t)(3 * m - k)) / ((seq_t)(k + 1));
            binom_m = binom_m * ((seq_t)(m - (seq_t)k - 1)) / ((seq_t)(k + 2));
        }
        return sum / m;
    }
}

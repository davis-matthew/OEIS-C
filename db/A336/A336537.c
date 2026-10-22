/*
 * A336537  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n^2+k+1,n)/(n^2+k+1).
 * https://oeis.org/A336537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{k=1..n} 2^k * binomial(n,k) * binomial(n²,k-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0  (a(0) = 1) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Use the widest available integer to keep intermediate values exact
       for the 64‑bit builds.  For larger 'seq_t' types we rely on the
       built‑in arithmetic (big integer, uint128, etc.). */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* 128‑bit intermediate arithmetic */
        unsigned __int128 N          = (unsigned __int128)n * n;   /* n²                */
        unsigned __int128 b1          = 1;  /* C(n,0)              */
        unsigned __int128 b2          = 1;  /* C(n²,0)             */
        unsigned __int128 pow2        = 2;  /* 2^1                 */
        unsigned __int128 sum         = 0;  /* running sum          */

        for (idx_t k = 1; k <= n; ++k) {
            /* Update C(n,k) */
            b1 = b1 * (unsigned __int128)(n - k + 1) / (unsigned __int128)k;

            /* Term = 2^k * C(n,k) * C(n²,k-1) */
            unsigned __int128 term = b1 * b2 * pow2;
            sum += term;

            /* Prepare C(n²,k) for the next iteration */
            if (k < n) {
                b2 = b2 * (N - (unsigned __int128)(k - 1)) / (unsigned __int128)k;
            }

            /* Next power of two */
            pow2 <<= 1;
        }

        /* a(n) = sum / n */
        return (seq_t)(sum / (unsigned __int128)n);
    } else {
        /* Full precision arithmetic for larger seq_t (uint128 or big integer) */
        seq_t N      = (seq_t)n * (seq_t)n;    /* n² */
        seq_t b1     = (seq_t)1;  /* C(n,0)             */
        seq_t b2     = (seq_t)1;  /* C(n²,0)            */
        seq_t pow2   = (seq_t)2;  /* 2^1                */
        seq_t sum    = (seq_t)0;  /* running sum         */

        for (idx_t k = 1; k <= n; ++k) {
            /* Update C(n,k) */
            b1 = b1 * (seq_t)(n - k + 1) / (seq_t)k;

            /* Term = 2^k * C(n,k) * C(n²,k-1) */
            seq_t term = b1 * b2 * pow2;
            sum = sum + term;

            /* Prepare C(n²,k) for next iteration */
            if (k < n) {
                b2 = b2 * (N - (seq_t)(k - 1)) / (seq_t)k;
            }

            /* Next power of two */
            pow2 = pow2 * 2;
        }

        /* a(n) = sum / n */
        return sum / (seq_t)n;
    }
}

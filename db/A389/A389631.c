/*
 * A389631  Expansion of (1/x) * Series_Reversion( x / (1 + x^2 * (1 + x))^2
 *          ).
 * https://oeis.org/A389631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Σ_{k=0..⌊n/2⌋} binomial(2*(n+1),k) *
 *            binomial(k, n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the binomial utility provided in the
   build. For 64‑bit seq_t types an unsigned __int128 accumulator is
   employed to avoid intermediate overflow. For larger seq_t types
   (arbitrary‑precision) the sum is performed in seq_t directly. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m = n + 1;                    /* (n+1) used in numerator and divisor       */
    idx_t maxk = n >> 1;                /* floor(n/2)                                */

    /* Branch for built‑in integer types (max 64 bits) */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;

        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t term1 = binomial(2 * m, k);
            idx_t sub   = n - (k << 1);
            seq_t term2 = binomial(k, sub);

            sum += (unsigned __int128)term1 * (unsigned __int128)term2;
        }

        return (seq_t)(sum / (unsigned __int128)m);
    }

    /* Branch for larger or arbitrary‑precision seq_t */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(2 * m, k);
        idx_t sub   = n - (k << 1);
        seq_t term2 = binomial(k, sub);

        sum = sum + term1 * term2;
    }

    return sum / (seq_t)m;
}

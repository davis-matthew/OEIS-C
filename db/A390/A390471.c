/*
 * A390471  a(n) = Sum_{k=0..n} binomial(n+3*k+2,k).
 * https://oeis.org/A390471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} binomial(n+3*k+2, k)
 * It uses a multiplicative binomial computation that reduces
 * intermediate values by the greatest common divisor to avoid
 * overflow on 64‑bit implementations while still working with
 * arbitrary‑precision seq_t types.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t binom_multiplicative(idx_t a, idx_t b)
{
    if (b > a - b) b = a - b;          /* use the smaller of k and n-k */
    seq_t result = 1;

    for (idx_t i = 1; i <= b; ++i) {
        seq_t numer = a - b + i;
        seq_t denom = i;

        /* cancel common factors between result and denom */
        seq_t g = gcd(result, denom);
        result /= g;
        denom /= g;

        /* cancel remaining common factors between numer and denom */
        g = gcd(numer, denom);
        numer /= g;
        denom /= g;

        /* denom should be 1 now; if not, cancel with result again */
        if (denom != 1) {
            g = gcd(result, denom);
            result /= g;
            denom /= g;
        }

        /* Now multiply result by numer (safe on arbitrary precision) */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        /* For C11, use int128 if available */
        __int128 temp = (__int128)result * numer;
        result = (seq_t)temp;
#else
        /* Fallback: rely on big‑integer seq_t operations */
        result = result * numer;
#endif
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t N = n + 3 * k + 2;
        seq_t term = binom_multiplicative(N, k);
        sum += term;
    }
    return sum;
}

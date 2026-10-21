/*
 * A282077  Number of 9-element subsets of [n+9] having an even sum.
 * https://oeis.org/A282077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k even} binomial(odd(n+9),k) * binomial(even(n+9),9−k)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 47, u64 n <= 614,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count number of 9‑element subsets of [n+9] having an even sum */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m = n + (idx_t)9;
    idx_t odds  = (m + (idx_t)1) >> 1;   /* floor((m+1)/2) */
    idx_t evens = m >> 1;                /* floor(m/2)     */

    seq_t result = 0;
    for (int k = 0; k <= 8; k += 2) {
        if (odds < (idx_t)k || evens < (idx_t)(9 - k))
            continue;
        seq_t term = binomial(odds, (idx_t)k) * binomial(evens, (idx_t)(9 - k));
        result += term;
    }
    return result;
}

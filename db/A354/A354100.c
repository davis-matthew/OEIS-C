/*
 * A354100  The 3-adic valuation of sigma, sum of divisors function.
 * https://oeis.org/A354100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₃(σ(n)) (the 3‑adic valuation of the sum‑of‑divisors
 *            function)
 * Range:     u8 n <= 1121, u16 n <= 7855, u32 n <= 7855, u64 n <= 7855,
 *            u128 n <= 7855, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);            /* σ(n) */
    seq_t v = 0;                      /* v₃(σ(n)) */
    while ((s % (seq_t)3) == 0) {
        s /= (seq_t)3;
        ++v;
    }
    return v;
}

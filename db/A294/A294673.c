/*
 * A294673  Order of the "inside-out" permutation on 2n+1 letters.
 * https://oeis.org/A294673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest k>0 such that (-2*(-1)^n)^k ≡ ±1 (mod 4n+3)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* 4n+3 is always odd and fits in 64 bits for the intended n range. */
    uint64_t M = (uint64_t)4 * (uint64_t)n + 3ULL;
    seq_t  M_seq = (seq_t)M;

    /* t = -2*(-1)^n  (mod M) */
    seq_t t_mod = (n & 1ULL) ? (seq_t)2 : (M_seq - (seq_t)2);

    /* Compute Euler's totient φ(M) with a simple trial division. */
    uint64_t phi   = M;
    uint64_t mtmp  = M;
    for (uint64_t p = 2; p * p <= mtmp; ++p) {
        if (mtmp % p == 0) {
            phi = phi / p * (p - 1);
            while (mtmp % p == 0) mtmp /= p;
        }
    }
    if (mtmp > 1)           /* remaining prime factor */
        phi = phi / mtmp * (mtmp - 1);

    /* Compute the multiplicative order r of t_mod modulo M. */
    uint64_t r = phi;
    uint64_t tempr = phi;
    for (uint64_t p = 2; p * p <= tempr; ++p) {
        if (tempr % p == 0) {
            /* Reduce r by successive powers of p while still keeping
               t_mod^r ≡ 1 (mod M). */
            while (r % p == 0) {
                uint64_t cand = r / p;
                if (mod_pow(t_mod, cand, M_seq) == (seq_t)1)
                    r = cand;
                else
                    break;
            }
            while (tempr % p == 0) tempr /= p;
        }
    }
    if (tempr > 1) {
        uint64_t p = tempr;
        while (r % p == 0) {
            uint64_t cand = r / p;
            if (mod_pow(t_mod, cand, M_seq) == (seq_t)1)
                r = cand;
            else
                break;
        }
    }

    /* If r is even and t_mod^(r/2) ≡ -1 (mod M), the true order
       on the quotient (Z/MZ)/{±1} is r/2. */
    if ((r & 1ULL) == 0) {
        seq_t mid = mod_pow(t_mod, r / 2, M_seq);
        if (mid == (seq_t)(M - 1))
            return (seq_t)(r / 2);
    }

    return (seq_t)r;
}

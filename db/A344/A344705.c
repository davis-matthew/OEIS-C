/*
 * A344705  a(n) = n + A001615(n) - sigma(n), where A001615 is the Dedekind
 *          psi-function, and sigma(n) gives the sum of divisors of n;
 *          difference between psi and the sum of proper divisors.
 * https://oeis.org/A344705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n + Dedekind ψ(n) – σ(n)
 * Range:     i8 n <= 48, i16 n <= 1848, i32 n <= 8976, i64 n <= 8976,
 *            i128 n <= 8976, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Dedekind ψ(n) = n × ∏_{p|n}(1 + 1/p) and σ(n) = ∑_{d|n}d
   by prime factorisation of n using trial division.  */ 

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1‑based offset: a(1) is the first term.  */
    if (n == 0) return (seq_t)0;   /* defensive guard */

    seq_t psi   = (seq_t)n;   /* Dedekind ψ(n)  */
    seq_t sigma = (seq_t)1;   /* σ(n)          */
    idx_t tmp   = n;          /* remaining factor */

    /* Factor the number n.  */
    /* Handle factor 2 separately to avoid extra work. */
    if ((tmp & 1) == 0) {                /* tmp even? */
        idx_t exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            exp++;
        }
        /* ψ update: multiply by (p+1) / p  */
        psi = psi * ((seq_t)3) / (seq_t)2;
        /* σ update: multiply by (p^(exp+1)-1)/(p-1)  */
        seq_t p_pow = (seq_t)1;
        for (idx_t i = 0; i < exp + 1; ++i) p_pow *= (seq_t)2;
        sigma *= (p_pow - (seq_t)1);
    }

    /* Trial division for odd primes.  */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            /* ψ update */
            psi = psi * ((seq_t)p + (seq_t)1) / (seq_t)p;
            /* σ update */
            seq_t p_pow = (seq_t)1;
            for (idx_t i = 0; i < exp + 1; ++i) p_pow *= (seq_t)p;
            sigma *= (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        }
    }

    /* Any remaining factor > 1 is a prime.  */
    if (tmp > 1) {
        seq_t p = (seq_t)tmp;
        psi = psi * (p + (seq_t)1) / p;
        sigma *= (p + (seq_t)1);
    }

    /* Final formula: a(n) = n + ψ(n) – σ(n).  */
    seq_t result = (seq_t)n + psi - sigma;
    return result;
}

/*
 * A329885  a(n) = A051903(n) mod A002322(n), where A051903 gives the maximal
 *          prime exponent of n, and A002322 is Carmichael's lambda (also
 *          known as psi).
 * https://oeis.org/A329885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051903(n) mod A002322(n) (maximal prime exponent mod
 *            Carmichael's λ)
 * Range:     u8 n <= 9379, u16 n <= 9379, u32 n <= 9379, u64 n <= 9379,
 *            u128 n <= 9379, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Computes the OEIS sequence A329885 */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* the sequence is defined for n ≥ 1, but return 0 for safety */
        return 0;
    }

    uint64_t m = (uint64_t)n;        /* mutable factorisation of n */
    seq_t g = 0;                     /* maximal prime exponent */
    seq_t lam = 1;                   /* Carmichael λ of n */

    /* --- factor 2 separately --- */
    unsigned count = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++count;
    }

    if (count > 0) {
        if (count > g) {
            g = count;
        }

        seq_t part;
        if (count == 1)             part = 1;
        else if (count == 2)        part = 2;
        else                        part = ((seq_t)1) << (count - 2); /* 2^(k‑2) */

        seq_t d = gcd(lam, part);
        lam = (lam / d) * part;     /* lcm */
    }

    /* --- factor odd primes --- */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        unsigned exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp == 0) {
            continue;
        }

        if (exp > g) {
            g = exp;
        }

        /* λ(p^exp) = p^(exp‑1) * (p‑1)   for odd primes */
        seq_t p_power = 1;
        for (unsigned i = 1; i < exp; ++i) {
            p_power *= (seq_t)p;      /* p^(exp‑1) */
        }
        seq_t part = p_power * ((seq_t)p - 1);

        seq_t d = gcd(lam, part);
        lam = (lam / d) * part;     /* lcm */
    }

    /* --- remaining prime factor (if any) --- */
    if (m > 1) {
        if (1 > g) {
            g = 1;
        }
        seq_t part = (seq_t)m - 1;   /* λ(m) = m‑1 for prime m */
        seq_t d = gcd(lam, part);
        lam = (lam / d) * part;     /* lcm */
    }

    /* --- result --- */
    return g % lam;
}

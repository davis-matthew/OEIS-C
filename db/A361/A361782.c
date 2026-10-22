/*
 * A361782  Numerators of the harmonic means of the bi-unitary divisors of the
 *          positive integers.
 * https://oeis.org/A361782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( n*A286324(n)/A188999(n) )
 * Range:     u8 n <= 12, u16 n <= 750, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via multiplicative formula for bi‑unitary harmonic mean */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                         /* offset starts at 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t num_total = 1;
    seq_t den_total = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t p_seq = (seq_t)p;
        seq_t e_seq = (seq_t)e;

        /* t = e+1 if e odd, else e */
        seq_t t = ((e & 1) != 0) ? (e_seq + 1) : e_seq;

        /* compute powers */
        seq_t pow_pe  = big_pow(p_seq, e_seq);          /* p^e  */
        seq_t pow_pe1 = big_pow(p_seq, e_seq + 1);      /* p^(e+1) */

        /* numerator of fraction for this prime power */
        seq_t num_pref = pow_pe * (p_seq - 1) * t;      /* t * p^e * (p-1) */

        /* denominator of fraction for this prime power */
        seq_t den_pref;
        if (e & 1) { /* e odd */
            den_pref = pow_pe1 - 1;                    /* p^(e+1) - 1 */
        } else { /* e even */
            seq_t p_half = big_pow(p_seq, e_seq / 2);          /* p^(e/2) */
            seq_t sub = (p_seq - 1) * p_half;                 /* (p-1) * p^(e/2) */
            den_pref = pow_pe1 - 1 - sub;                     /* p^(e+1) -1 - (p-1)p^(e/2) */
        }

        /* reduce the fraction for this prime power */
        seq_t g = gcd(num_pref, den_pref);
        seq_t num_i = num_pref / g;
        seq_t den_i = den_pref / g;

        /* multiply into the global product */
        num_total *= num_i;
        den_total *= den_i;

        /* reduce the accumulated fraction */
        seq_t g2 = gcd(num_total, den_total);
        num_total /= g2;
        den_total /= g2;
    }

    /* the numerator of the harmonic mean is the desired term */
    return num_total;
}

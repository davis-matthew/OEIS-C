/*
 * A337337  a(n) = gcd(1+sigma(s), (s+1)/2), where s is the square of n once
 *          prime-shifted (s = A003961(n)^2 = A003961(n^2)).
 * https://oeis.org/A337337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( (s+1)/2 , 1+sigma(s) ) * where s = A003961(n)^2 and
 *            A003961 is the multiplicative * function that replaces each
 *            prime factor p by the next prime * after p. That is, *
 *            A003961(n) = Π next_prime(p)^{e_p} * for n = Π p^{e_p}.
 *            Consequently * s = ( Π next_prime(p)^{e_p} )^2 = Π
 *            next_prime(...
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 9142, u64 n <= 9142,
 *            u128 n <= 9142, big any n
 * Helpers:   factor_u64, gcd, next_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;   /* undefined for n < 1 */
    }

    /* ---------- factorise n ---------- */
    /* maximal number of prime factors for 64‑bit n is 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* ---------- compute A003961(n) ---------- */
    seq_t ap = (seq_t)1;   /* product of next_prime(p)^e */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        int     e  = (int)exps[i];
        uint64_t q = next_prime(p);   /* next prime after p */

        /* q^e (as seq_t) */
        seq_t q_seq = (seq_t)q;
        seq_t q_pow = seq_pow(q_seq, (idx_t)e);
        ap *= q_pow;
    }

    /* ---------- compute s = ap^2 ---------- */
    seq_t s = ap * ap;

    /* ---------- t = (s+1)/2 ---------- */
    seq_t t = (s + 1) / 2;

    /* ---------- compute sigma(s) ---------- */
    /* sigma(s) = Π ( q^{2e+1} - 1 ) / (q-1)   for each prime factor q of AP */
    seq_t sigma_s = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = next_prime(primes[i]);      /* same q as used in ap */
        int     e = (int)exps[i];
        idx_t  exp = (idx_t)(2 * e + 1);          /* exponent in numerator */

        seq_t q_seq = (seq_t)q;

        seq_t num = seq_pow(q_seq, exp) - (seq_t)1;
        seq_t den = q_seq - (seq_t)1;            /* q-1 >= 1 for q >= 2 */

        /* (num / den) is guaranteed integer */
        seq_t factor = num / den;
        sigma_s *= factor;
    }

    /* ---------- final gcd ---------- */
    seq_t result = gcd(t, sigma_s + (seq_t)1);

    return result;
}

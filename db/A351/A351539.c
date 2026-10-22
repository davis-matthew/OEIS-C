/*
 * A351539  Number of odd prime factors p of n such that p^(1+valuation(n,p))
 *          divides sigma(n).
 * https://oeis.org/A351539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 9834, u32 n <= 9834, u64 n <= 9834,
 *            u128 n <= 9834, big any n
 * Helpers:   factor_u64_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351539: a(n) = number of odd prime factors p of n such that p^(1+v_p(n)) divides σ(n) */
/* The function works for any seq_t (unsigned integer or arbitrary‑precision) and any idx_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64_local.h"

/* Return a(n) for OEIS sequence A351539 */
seq_t a(idx_t n)
{
    /* factorisation buffers – enough for 64‑bit numbers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pcnt;
    factor_u64_local((uint64_t)n, primes, exps, &pcnt);

    /* compute σ(n) = product over prime powers of (p^(a+1)-1)/(p-1) = 1+p+…+p^a */
    seq_t sigma = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t a  = exps[i];
        seq_t   term = 1;
        seq_t   mult = 1;
        for (uint64_t j = 1; j <= a; ++j) {
            mult *= (seq_t)p;
            term += mult;
        }
        sigma *= term;
    }

    /* count the odd primes for which p^(e+1) divides σ(n) */
    seq_t count = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p  = primes[i];
        if (p == 2) continue;                 /* skip even prime */
        uint64_t e  = exps[i];
        uint64_t need = e + 1;

        /* compute valuation of sigma at p */
        seq_t vp   = 0;
        seq_t t    = sigma;
        seq_t sp   = (seq_t)p;
        while (t % sp == 0) {
            t /= sp;
            vp++;
        }

        if (vp >= need) count++;
    }

    return count;
}

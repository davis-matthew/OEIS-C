/*
 * A262095  Number of non-semiprime divisors of n.
 * https://oeis.org/A262095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(n) - A086971(n) = (number of divisors of n) -
 *            (number of semiprime divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts the non‑semiprime divisors of n.  A divisor is semiprime if it
   is the product of exactly two primes (with multiplicity).  For the
   factorisation n = Π p_i^e_i, the number of semiprime divisors equals
   (# of i with e_i≥2) + C(k,2) where k is the number of distinct primes.
   Consequently,
         a(n) = Π (e_i+1)  -  (cnt_sq + k*(k-1)/2).        */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Convert index to 64‑bit integer for factorisation */
    uint64_t nn = (uint64_t)n;

    /* Buffers for prime factors and exponents.
       64 is more than enough for 64‑bit numbers. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    /* Factorise n.  cnt will contain the number of distinct primes. */
    factor_u64(nn, pr, ex, &cnt);

    /* Compute τ(n) = Π (e_i+1) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (seq_t)(ex[i] + 1);
    }

    /* Count semiprime divisors.
       (1) squares p^2 when e_i ≥ 2
       (2) products p_i * p_j for i < j when both e_i, e_j ≥ 1   */
    seq_t semiprime = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] >= 2) {
            semiprime += (seq_t)1;
        }
    }
    seq_t k = (seq_t)cnt;
    semiprime += k * (k - 1) / 2;

    /* Non‑semiprime divisor count */
    return tau - semiprime;
}

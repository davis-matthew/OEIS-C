/*
 * A381924  Multiplicative order of n mod prime(n).
 * https://oeis.org/A381924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of n modulo prime(n)
 * Range:     u8 n <= 54, u16 n <= 3500, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

/* Computes the least k such that prime(n) divides n^k – 1.
   prime(n) is obtained via get_prime(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for unexpected calls */

    /* Obtain the n-th prime (as uint32_t) and promote to 64‑bit. */
    uint32_t p_u32 = get_prime(n);
    uint64_t p     = (uint64_t)p_u32; 

    /* Factor p – 1 into distinct prime factors. */
    uint64_t m = p - 1;
    uint64_t fac[64];
    size_t fac_cnt = 0;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            fac[fac_cnt++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) fac[fac_cnt++] = m;

    /* Compute multiplicative order. */
    uint64_t order = p - 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t q = fac[i];
        while (order % q == 0) {
            uint64_t exp = order / q;
            if (mod_pow((seq_t)n, exp, (seq_t)p) == (seq_t)1)
                order = exp;
            else
                break;
        }
    }

    return (seq_t)order;
}

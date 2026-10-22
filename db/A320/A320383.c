/*
 * A320383  Multiplicative order of 3/2 modulo n-th prime.
 * https://oeis.org/A320383
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the multiplicative order of 3/2 modulo the n‑th prime
 *            (offset 3)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/get_prime.h"

/* Compute the multiplicative order of 3/2 modulo prime p (p >= 5). */
static seq_t order_3over2_prime(uint32_t p) {
    seq_t mod = (seq_t)p;
    /* 2^(-1) mod p via Fermat: 2^(p-2) ≡ 2^(-1) (mod p) */
    seq_t inv2 = mod_pow((seq_t)2, (uint64_t)p - 2, mod);
    seq_t t    = ((seq_t)3 * inv2) % mod;          /* t ≡ 3·2^(−1) (mod p) */

    seq_t order = mod - 1;                         /* start with p-1 */
    uint64_t temp = p - 1;                         /* for factorisation */

    for (uint64_t q = 2; q * q <= temp; ++q) {
        if (temp % q == 0) {
            /* record q as prime factor */
            while (temp % q == 0) temp /= q;
            /* try to reduce order by powers of q */
            while (order % (seq_t)q == 0) {
                seq_t cand = order / q;
                if (mod_pow(t, (uint64_t)cand, mod) == (seq_t)1)
                    order = cand;
                else
                    break;
            }
        }
    }
    /* remaining factor > sqrt(temp) */
    if (temp > 1) {
        uint64_t q = temp;
        while (order % (seq_t)q == 0) {
            seq_t cand = order / q;
            if (mod_pow(t, (uint64_t)cand, mod) == (seq_t)1)
                order = cand;
            else
                break;
        }
    }
    return order;
}

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;     /* undefined below offset, return 0 */
    uint32_t p = get_prime(n);      /* p = n-th prime (p ≥ 5 for n ≥ 3) */
    return order_3over2_prime(p);
}

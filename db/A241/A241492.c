/*
 * A241492  a(n) = |{0 < g < prime(n): g is a primitive root modulo prime(n)
 *          and g is a product of two consecutive integers}|.
 * https://oeis.org/A241492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of primitive roots g modulo prime(n) with * g
 *            = k*(k+1) for some integer k, 0 < g < prime(n). The function *
 *            evaluates this definition directly by brute force, which is fast
 *            * enough for n ≤ 10⁴. Primitive_root test uses the criterion
 *            that * g^((p−1)/q) ≠ 1 (mod p) for every di...
 * Range:     u8 n <= 54, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow, unique_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/unique_prime_factors.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* prime(n) fits in 32-bit for n ≤ 10000, but use seq_t for modulus */
    uint32_t pr = get_prime(n);
    uint64_t p = (uint64_t)pr;          /* full prime as 64‑bit */
    seq_t p_mod = (seq_t)p;             /* for mod_pow operations */

    /* factor p-1 */
    uint64_t factors[64];
    size_t fac_cnt;
    unique_prime_factors(p - 1, factors, &fac_cnt);

    seq_t count = 0;

    /* iterate k such that k*(k+1) < p */
    for (idx_t k = 1; ; ++k) {
        unsigned __int128 prod = (unsigned __int128)k * (unsigned __int128)(k + 1);
        if (prod >= p) break;           /* no larger product will work */

        /* check if prod is a primitive root mod p */
        bool primitive = true;
        for (size_t i = 0; i < fac_cnt && primitive; ++i) {
            uint64_t exp = (p - 1) / factors[i];
            seq_t res = mod_pow((seq_t)(prod % p), exp, p_mod);
            if (res == 1) primitive = false;
        }
        if (primitive) ++count;
    }

    return count;
}

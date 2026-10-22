/*
 * A395775  The twisted Euler phi-function for the Dirichlet character
 *          kronecker(-20,.) mod 20.
 * https://oeis.org/A395775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(2^e) = 2^e, a(5^e) = 5^e, a(p^e) =
 *            p^(e-1)*(p-1) if p ≡ 1,3,7,9 (mod 20), a(p^e) = p^(e-1)*(p+1) if
 *            p ≡ 11,13,17,19 (mod 20)
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t pow_uint(uint64_t base, int exp) {
    seq_t res = 1;
    seq_t b = (seq_t)base;
    for (int i = 0; i < exp; ++i) res *= b;
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)n;  /* a(1)=1, a(0)=0 (unused) */

    uint64_t nn = n;
    /* We use up to 64 distinct prime factors for 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = (int)exps[i];
        seq_t term;

        if (p == 2 || p == 5) {
            term = pow_uint(p, e);          /* p^e */
        } else {
            /* Compute p^(e-1) */
            seq_t p_pow = 1;
            seq_t bp = (seq_t)p;
            for (int j = 1; j < e; ++j) p_pow *= bp;  /* e-1 multiplications */

            if (p % 20 == 1 || p % 20 == 3 || p % 20 == 7 || p % 20 == 9) {
                term = p_pow * ((seq_t)(p - 1));
            } else {
                term = p_pow * ((seq_t)(p + 1));
            }
        }
        result *= term;
    }
    return result;
}

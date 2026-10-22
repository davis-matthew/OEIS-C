/*
 * A371242  The sum of the unitary divisors of n that are cubefree numbers
 *          (A004709).
 * https://oeis.org/A371242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} ( e≤2 ? p^e+1 : 1 ) (multiplicative: if n = Π
 *            p_i^e_i then a(n) = Π a(p_i^e_i) with a(p^e) = p^e+1 for e≤2 and
 *            a(p^e)=1 for e≥3. ) Computes the sum of unitary divisors of n
 *            that are cubefree numbers
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* factorization of n: primes[i]^exps[i], i=0..cnt-1 */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t  term;

        if (e <= 2) {
            seq_t p_pow = (seq_t)1;
            for (uint64_t j = 0; j < e; ++j) {
                p_pow *= (seq_t)p;
            }
            term = p_pow + (seq_t)1;
        } else {
            term = (seq_t)1;
        }

        result *= term;
    }
    return result;
}

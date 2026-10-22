/*
 * A368329  The largest term of A054743 that divide n.
 * https://oeis.org/A368329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n, e>p} p^e (product of all prime powers whose
 *            exponent exceeds the prime) Multiplicative with a(p^e) = 1 if e
 *            ≤ p, a(p^e) = p^e if e > p
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Factor n into primes.  factor_u64 returns the count of distinct primes
       and fills the arrays with the primes and their exponents.  The size
       of the arrays is more than enough for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e > p) {
            seq_t p_seq = (seq_t)p;
            seq_t pe = big_pow(p_seq, (seq_t)e);
            result *= pe;
        }
    }

    return result;
}

/*
 * A348963  a(n) is multiplicative with a(p^e) = Sum_{d|e} p^(e-d).
 * https://oeis.org/A348963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with a(p^e) = ∑_{d|e} p^(e-d)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* Factor n.  No more than 64 distinct primes for a 64‑bit number. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p       = (seq_t)primes[i];
        unsigned e    = (unsigned)exps[i];
        seq_t sum     = (seq_t)0;

        for (unsigned d = 1; d <= e; ++d) {
            if (e % d == 0) {                       /* d|e */
                unsigned power = e - d;              /* e-d ≤ e ≤ 63 */
                seq_t term = big_pow(p, (seq_t)power);
                sum += term;
            }
        }
        result *= sum;
    }
    return result;
}

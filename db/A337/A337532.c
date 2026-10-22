/*
 * A337532  a(n) = Sum_{d1|n, d2|n, d1<=d2} [(d1 mod 2) = (d2 mod 2)], where [
 *          ] is the Iverson bracket.
 * https://oeis.org/A337532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of unordered pairs (d1,d2) of divisors of n with
 *            d1 <= d2 and d1, d2 having the same parity. Computed as a(n) =
 *            C_odd + C_even where C_odd = O*(O+1)/2, C_even = E*(E+1)/2, O =
 *            number of odd divisors of n, E = number of even divisors of n
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* local types: idx_t is unsigned 64-bit, seq_t is arbitrary width */
    uint64_t N = (uint64_t)n;
    /* maximum distinct prime factors for 64‑bit numbers is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    seq_t tau = 1;          /* total number of divisors */
    seq_t tau_odd = 1;      /* number of odd divisors */
    int has_two = 0;        /* whether prime 2 appears */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t mult = (seq_t)exps[i] + 1;   /* (exp_i + 1) */
        tau *= mult;
        if (primes[i] == 2ULL) {
            has_two = 1;
            /* odd divisors are unaffected by factor 2 */
        } else {
            tau_odd *= mult;
        }
    }

    seq_t O = tau_odd;          /* odd divisor count */
    seq_t E = tau - O;          /* even divisor count */

    /* compute final result: O*(O+1)/2 + E*(E+1)/2 */
    seq_t res = O * (O + 1) / 2 + E * (E + 1) / 2;
    return res;
}

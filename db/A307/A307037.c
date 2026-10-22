/*
 * A307037  The unitary analog of the alternating sum-of-divisors function
 *          (A206369).
 * https://oeis.org/A307037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^e + (-1)^e)
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(p^e) = p^e + (-1)^e  for prime powers   */
#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64: factorizes uint64_t */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* should not happen for OEIS offset 1 */
    uint64_t nn = (uint64_t)n;

    if (nn == 1) return 1;                     /* a(1) = 1 */

    /* factorise nn */
    size_t cnt = 0;
    uint64_t primes[64], exps[64];             /* enough for uint64_t */
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = primes[i];
        size_t e = (size_t)exps[i];

        /* compute p^e */
        seq_t pe = 1;
        for (unsigned j = 0; j < e; ++j) pe *= p;

        /* add (-1)^e */
        seq_t term = pe + ((e % 2) == 0 ? (seq_t)1 : (seq_t)(-1));

        result *= term;
    }
    return result;
}

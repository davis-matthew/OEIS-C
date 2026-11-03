/*
 * A363920  a(n) = n^(tpf(n) * dpf(n)), where tpf(n) is the total number of
 *          prime factors of n if n >= 2 and otherwise = 0; dpf(n) is the
 *          number of distinct prime factors of n if n >= 2 and otherwise = 0.
 * https://oeis.org/A363920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(tpf(n) * dpf(n))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  tpf(n)  – total number of prime factors of n (with multiplicity)  */
/*  dpf(n)  – number of distinct prime factors of n                     */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* n < 2 ⇒ a(n) = 1 by definition (0^0 and 1^0 → 1) */
    if (n < 2)
        return (seq_t)1;

    /* factorize n; maximum number of distinct primes below 2^64 is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    unsigned tpf = 0;  /* total prime factors (with multiplicity)   */
    unsigned dpf = 0;  /* distinct prime factors                    */
    for (size_t i = 0; i < cnt; ++i) {
        tpf += (unsigned)exps[i];
        ++dpf;
    }

    unsigned exp = tpf * dpf;
    return ipow((seq_t)n, (seq_t)exp);
}

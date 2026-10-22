/*
 * A360458  Two times the median of the set of distinct prime factors of n;
 *          a(1) = 2.
 * https://oeis.org/A360458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * median of distinct prime factors of n; a(1)=2
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)2;

    /* Buffers for factorization (enough for any uint64_t) */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt == 0)
        return (seq_t)2;                /* Should not occur for n>1 */

    if (cnt == 1)
        return (seq_t)primes[0] * (seq_t)2;

    if (cnt & 1) {                       /* odd number of distinct primes */
        size_t mid = cnt >> 1;
        return (seq_t)primes[mid] * (seq_t)2;
    } else {                             /* even number of distinct primes */
        size_t i = cnt >> 1;
        return (seq_t)primes[i - 1] + (seq_t)primes[i];
    }
}

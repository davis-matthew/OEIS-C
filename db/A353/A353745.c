/*
 * A353745  Number of runs in the ordered prime signature of n.
 * https://oeis.org/A353745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of runs in the ordered prime signature of n
 * Range:     u8 n <= 8947, u16 n <= 8947, u32 n <= 8947, u64 n <= 8947,
 *            u128 n <= 8947, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 0, as the prime signature of 1 is empty                       */
    if (n < 2) return (seq_t)0;

    /* Arrays sufficient for any 64‑bit integer (≤64 distinct primes).      */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt;

    /* Factorise n; pr contains distinct primes in increasing order,       */
    /* ex contains the corresponding exponents.                            */
    factor_u64((uint64_t)n, pr, ex, &cnt);
    if (cnt == 0) return (seq_t)0;          /* safety for n = 1            */

    seq_t runs = 1;                         /* first run                  */
    for (size_t i = 1; i < cnt; ++i) {
        if (ex[i] != ex[i - 1]) ++runs;     /* start a new run if exponent |
                                             * changes                          */
    }
    return runs;
}

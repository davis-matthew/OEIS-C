/*
 * A358170  Heinz number of the partial sums of the n-th composition in
 *          standard order (A066099).
 * https://oeis.org/A358170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{s∈partial sums of the n-th composition in standard
 *            order} prime(s) (Heinz number of the partition given by the
 *            partial sums)
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t) -> 32‑bit prime */

seq_t a(idx_t n)
{
    if (n == 0)            /* empty composition → Heinz number 1 */
        return (seq_t)1;

    /* collect 1‑bit positions (1‑based, little‑endian) */
    uint64_t pos[65];
    uint64_t cnt = 0;
    idx_t k = n;
    for (uint64_t i = 1; k; ++i) {
        if (k & 1ULL)
            pos[cnt++] = i;   /* position i is set */
        k >>= 1;
    }

    /* differences of [0] ∪ pos[] give the parts of the composition */
    uint64_t part[65];
    for (uint64_t i = 0; i < cnt; ++i) {
        uint64_t prev = (i == 0) ? 0 : pos[i - 1];
        part[i] = pos[i] - prev;
    }

    /* compute partial sums in reverse order (standard Order) */
    seq_t prod = (seq_t)1;
    seq_t sum  = (seq_t)0;
    for (size_t i = cnt; i > 0; ) {
        --i;
        sum += (seq_t)part[i];
        prod *= (seq_t)get_prime((idx_t)sum);
    }

    return prod;
}

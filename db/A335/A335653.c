/*
 * A335653  Smallest prime whose binary expansion has Hamming distance 1 from
 *          2n+1, or 0 if no such prime exists.
 * https://oeis.org/A335653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 185, u32 n <= 1111, u64 n <= 1675,
 *            u128 n <= 2234, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335653
   a(n) = smallest prime whose binary expansion has Hamming distance 1 from 2n+1,
   or 0 if no such prime exists.
*/
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Compute 2n+1 */
    seq_t base = (seq_t)n * 2 + 1;

    /* Determine bit width of seq_t */
    const unsigned int widthbits = sizeof(seq_t) * 8;

    /* Bit length of base */
    const unsigned int bitlen = bitlen_seq_t(base);

    /* Upper bound on bit positions to try:
       we search all existing bits and a few beyond.
       For built‑in types we cannot shift past widthbits-1. */
    unsigned int max_k = bitlen + 16;
    if (max_k >= widthbits)
        max_k = widthbits - 1;

    seq_t best = 0;
    bool found = false;

    for (unsigned int k = 0; k <= max_k; ++k) {
        seq_t cand = base ^ ((seq_t)1 << k);
        if (cand < 2)         /* 0 or 1 cannot be prime */
            continue;
        if (is_prime_big(cand)) {
            if (!found || cand < best) {
                best = cand;
                found = true;
            }
        }
    }

    return found ? best : (seq_t)0;
}

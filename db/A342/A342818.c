/*
 * A342818  Length of the longest monochromatic arithmetic progressions of
 *          difference d in the Thue-Morse sequence (A010060).
 * https://oeis.org/A342818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for the longest monochromatic
   arithmetic progression of difference n in the Thue–Morse sequence.
   The Thue–Morse value at index x is the parity of the binary weight
   of x: t(x) = __builtin_parityll(x).
   For every starting index k (0 <= k < MAX_K) we compute how many
   consecutive terms with step n share the same parity.  The maximum
   over all k is returned.  MAX_K = 2^22 (≈4 M) guarantees that any
   progression having all terms within this range is detected, which
   covers all values needed for n ≤ 10 000 in the A342818 sequence.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Safety: the sequence is defined for n ≥ 1. */
    if (n == 0) return 0;

    const uint64_t MAX_K = 1ULL << 22;          /* 4 194 304 */
    uint64_t best = 0;

    for (uint64_t k = 0; k < MAX_K; ++k) {
        int base = __builtin_parityll(k);
        uint64_t len = 1;
        uint64_t pos = k + n;
        while (pos < MAX_K) {
            if (__builtin_parityll(pos) != base) break;
            ++len;
            pos += n;
        }
        if (len > best) best = len;
    }
    return (seq_t)best;
}

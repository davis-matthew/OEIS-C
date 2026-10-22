/*
 * A331803  a(n) is the largest positive integer occurring, when written in
 *          binary, as a substring in both binary n and binary n+1.
 * https://oeis.org/A331803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest integer (by value) whose binary representation
 *            appears as a contiguous substring of both binary n and binary
 *            n+1
 * Range:     u8 n <= 511, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 */
    if (n == 0)
        return (seq_t)0;

    idx_t m = n + 1;

    /* lengths of binary representations (no leading zeros) */
    unsigned lenN = floor_log2_uint64(n) + 1;
    unsigned lenM = floor_log2_uint64(m) + 1;

    /* bit arrays: most significant bit first */
    uint8_t bitsN[64], bitsM[64];
    for (unsigned i = 0; i < lenN; ++i)
        bitsN[i] = (uint8_t)((n >> (lenN - 1 - i)) & 1);
    for (unsigned i = 0; i < lenM; ++i)
        bitsM[i] = (uint8_t)((m >> (lenM - 1 - i)) & 1);

    /* Build a lookup table of all substrings of m */
    size_t maxValM = ((size_t)1 << lenM);       /* 2^lenM */
    bool *has = (bool *)calloc(maxValM, 1);
    for (unsigned i = 0; i < lenM; ++i) {
        size_t v = 0;
        for (unsigned j = i; j < lenM; ++j) {
            v = (v << 1) | bitsM[j];
            has[v] = true;
        }
    }

    /* Find the largest common substring value */
    seq_t best = 0;
    for (unsigned i = 0; i < lenN; ++i) {
        seq_t v = 0;
        for (unsigned j = i; j < lenN; ++j) {
            v = (v << 1) | bitsN[j];
            if (v <= (seq_t)(maxValM - 1) && has[(size_t)v]) {
                if (v > best)
                    best = v;
            }
        }
    }

    free(has);
    return best;
}

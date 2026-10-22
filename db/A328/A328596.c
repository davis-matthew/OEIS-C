/*
 * A328596  Numbers whose reversed binary expansion is a Lyndon word
 *          (aperiodic necklace).
 * https://oeis.org/A328596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer m such that the binary string of m,
 *            reversed to the same length, is a Lyndon word (lexicographically
 *            strictly smaller than all of its non‑trivial rotations)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Check whether the binary string of `x` reversed to the same length
   is a Lyndon word.  */
static bool
is_lyndon_candidate(seq_t x)
{
    /* length of binary representation (x > 0)                                    */
    size_t len = bitlen_seq_t(x);
    if (len == 0) return false;

    /* reverse the bits of `x` to get `rev` of the same length                */
    seq_t rev = 0;
    for (size_t i = 0; i < len; ++i) {
        rev = (rev << 1) | ((x >> i) & 1);
    }

    /* mask to keep only the lowest `len` bits                               */
    seq_t mask = ((seq_t)1 << len) - 1;

    /* check the Lyndon condition:  rev < rotated for all 1 <= k < len   */
    for (size_t k = 1; k < len; ++k) {
        seq_t rotated = ((rev << k) | (rev >> (len - k))) & mask;
        if (rev >= rotated) return false;  /* not strictly smaller */
    }
    return true;
}

/* Return the n-th term of the sequence (1‑based index).                 */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 1;
    while (1) {
        if (is_lyndon_candidate(cur)) {
            ++count;
            if (count == n) return cur;
        }
        ++cur;
    }
}

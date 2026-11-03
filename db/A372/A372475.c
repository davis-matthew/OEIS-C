/*
 * A372475  Length of binary expansion (or number of bits) of the n-th
 *          squarefree number.
 * https://oeis.org/A372475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bitlen_seq_t(A005117(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   bit_length, is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of bits in the binary expansion of the n-th squarefree integer */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *sq = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static idx_t cur = 1;

    /* Generate squarefree numbers until we have at least n of them. */
    while (sz < n) {
        if (is_squarefree((seq_t)cur)) {
            if (sz == cap) {
                cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, sq, cap);
                if (!tmp) abort();
                sq = tmp;
            }
            sq[sz++] = (seq_t)cur;
        }
        cur++;
    }

    return bit_length(sq[n - 1]);
}

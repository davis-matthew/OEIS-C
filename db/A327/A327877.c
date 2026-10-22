/*
 * A327877  Numbers having an odd number of non-unitary prime factors.
 * https://oeis.org/A327877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_odd_nonunitary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers having an odd number of non‑unitary prime factors.
   A number's non‑unitary prime factors are the primes that appear
   with exponent ≥ 2 in its prime factorisation. This routine
   generates the sequence in increasing order and returns the nth term
   (1‑based).  It memoises the terms in a static array so that calls
   may appear in any order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_odd_nonunitary.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    static seq_t *seq          = NULL;
    static size_t seq_cap      = 0;
    static idx_t seq_len       = 0;
    static uint64_t cur         = 2;          /* next integer to test */

    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < n) {
        if (has_odd_nonunitary(cur)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)cur;
        }
        cur++;
    }

    return seq[n - 1];   /* a(1) is stored at seq[0] */
}

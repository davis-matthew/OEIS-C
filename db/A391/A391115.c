/*
 * A391115  Numbers that are neither 4-free nor 4-full.
 * https://oeis.org/A391115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the sequence consists of integers that have at least one prime
 *            factor with exponent ≥ 4 and at least one prime factor with
 *            exponent ≤ 3 (i.e., a mixed exponent). a(1) = 48, a(2) = 80, …
 * Range:     u8 n <= 12, u16 n <= 4936, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_mixed_exponents
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_mixed_exponents.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static uint64_t last = 1;

    if (n == 0) return 0;  /* undefined for n = 0 */

    while (len < n) {
        ++last;
        if (has_mixed_exponents(last)) {
            if (len == cap) {
                cap = (cap == 0) ? 8 : cap * 2;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = (seq_t)last;
        }
    }
    return seq[n - 1];
}

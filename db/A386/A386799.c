/*
 * A386799  Numbers without an exponent 3 in their prime factorization.
 * https://oeis.org/A386799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 232, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_semi3_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: numbers whose prime factorizations have no exponent 3 (sequence A386799) */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_is_semi3_free.h"

/* Return the n-th term (1‑based) of the sequence A386799. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* unused, as offset is 1, but keep defensive */

    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        if (_is_semi3_free(candidate)) {
            ++count;
            if (count == n) break;
        }
        ++candidate;
    }
    return (seq_t)candidate;
}

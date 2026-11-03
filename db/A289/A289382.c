/*
 * A289382  a(n) = 2^n mod triangular(n).
 * https://oeis.org/A289382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n mod n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9787 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2; /* triangular number in seq_t */
    return powmod((seq_t)2, n, tri);          /* 2^n mod tri */
}

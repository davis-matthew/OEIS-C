/*
 * A325642  a(1) = 1; for n > 1, a(n) = k for the least divisor d > 1 of n
 *          such that A048720(d,k) = n for some k.
 * https://oeis.org/A325642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 9946, u32 n <= 9946, u64 n <= 9946,
 *            u128 n <= 9946, big any n
 * Helpers:   degree_u64, poly_divide_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325642: a(1)=1.  For n>1, a(n) is the quotient k such that
 *   the binary polynomial of the least divisor d>1 of n (d | n)
 *   divides the binary polynomial of n over GF(2).
 *   The quotient polynomial evaluated as a binary integer is k.
 *   If no proper divisor satisfies, the divisor is n itself and k=1.
 *
 * This implementation works for any seq_t (builtin or big integer)
 * and any idx_t (here uint64_t).  It avoids dynamic allocation of
 * seq_t objects, so it compiles in both C11 and C++17.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/degree_u64.h"
#include "../utils/poly_divide_u64.h"

/* Public function required by the checker. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1)=1 */

    uint64_t un = (uint64_t)n;
    /* Scan divisors in increasing order. */
    for (uint64_t d = 2; d <= un; ++d) {
        if (un % d != 0) continue;       /* not a divisor */
        seq_t q = poly_divide_u64(un, d);
        if (q != 0) return q;            /* first divisor that works */
    }
    /* If none found, n divides itself: return 1. */
    return (seq_t)1;
}

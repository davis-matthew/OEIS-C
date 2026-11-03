/*
 * A290098  Characteristic function for A003658 (fundamental discriminants of
 *          real quadratic fields).
 * https://oeis.org/A290098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8767 (2026-11-01)
 * Helpers:   is_squarefree_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A290098: a(n) = 1 if n is a fundamental discriminant of a real quadratic field
 * and 0 otherwise.  
 * A positive integer n is a fundamental discriminant (real quadratic) iff
 *   • n is odd, square‑free, and n ≡ 1 (mod 4), or
 *   • n ≡ 0 (mod 4), n/4 is square‑free, and n/4 ≡ 2 or 3 (mod 4).
 * The sequence starts: 1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1,...
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset 1; behaviour below offset irrelevant. */
    uint64_t m = (uint64_t)n;
    if (m == 0) return (seq_t)0;

    if ((m & 1ULL) != 0ULL) {  /* odd n */
        if ((m % 4ULL == 1ULL) && is_squarefree_u64(m))
            return (seq_t)1;
        else
            return (seq_t)0;
    } else {                  /* even n */
        if ((m % 4ULL) == 0ULL) {
            uint64_t d = m / 4ULL;
            if ((d % 4ULL == 2ULL || d % 4ULL == 3ULL) &&
                is_squarefree_u64(d))
                return (seq_t)1;
        }
        return (seq_t)0;
    }
}

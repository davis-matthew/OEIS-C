/*
 * A258385  a(n) = n^(n+1) * (n-1)^n.
 * https://oeis.org/A258385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n+1) * (n-1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Big integer helper from the provided utils
   big_pow(seq_t base, seq_t exp) returns base^exp as seq_t */
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Offset is 1, so a(1) = 0 */
    if (n <= 1)
        return (seq_t)0;

    seq_t base1 = (seq_t)n;
    seq_t exp1  = (seq_t)(n + 1);
    seq_t pow1  = ipow(base1, exp1);

    seq_t base2 = (seq_t)(n - 1);
    seq_t exp2  = (seq_t)n;
    seq_t pow2  = ipow(base2, exp2);

    return pow1 * pow2;
}

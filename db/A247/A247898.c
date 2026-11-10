/*
 * A247898  First differences of A247678.
 * https://oeis.org/A247898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A247678(n+1) - A247678(n), where A247678 lists the odd
 *            composites == 4 (mod 9), i.e. composites of the form 18*m+13;
 *            found by a linear scan
 * Range:     u8 n <= 5, u16 n <= 2548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 8457 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t m = 2;
    idx_t count = 0;
    seq_t first = 0;
    seq_t cand;
    for (;;) {
        cand = (seq_t)18 * (seq_t)m + (seq_t)13;
        if (!is_prime_seq(cand)) {
            count++;
            if (count == n) {
                first = cand;
            } else if (count == n + 1) {
                return cand - first;
            }
        }
        m++;
    }
}

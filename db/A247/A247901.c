/*
 * A247901  First differences of A247682.
 * https://oeis.org/A247901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A247682(n+1) - A247682(n), where A247682 lists the
 *            composite numbers 18k+5 (search)
 * Range:     u8 n <= 4, u16 n <= 3140, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9970 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    seq_t first = 0;

    for (;; ++k) {
        seq_t cand = (seq_t)18 * (seq_t)k + (seq_t)5;
        if (!is_prime_seq(cand)) {
            ++count;
            if (count == n) {
                first = cand;
            } else if (count == n + 1) {
                return cand - first;
            }
        }
    }
}

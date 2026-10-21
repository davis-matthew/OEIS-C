/*
 * A297132  Numbers having a down-first zigzag pattern in base 5; see
 *          Comments.
 * https://oeis.org/A297132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n-th integer whose base‑5 representation starts with a
 *            down step (first digit > second digit) and contains no equal
 *            consecutive digits. Computed by brute‑force search over integers
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_valid_down_first
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_valid_down_first.h"

seq_t a(idx_t n)
{
    idx_t hits = 0;
    for (idx_t v = 5; ; ++v) {          /* start from first admissible value */
        if (is_valid_down_first(v)) {
            ++hits;
            if (hits == n)
                return (seq_t)v;          /* cast to seq_t */
        }
    }
}

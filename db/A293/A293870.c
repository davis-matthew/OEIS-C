/*
 * A293870  Numbers having '10' as substring of their digits.
 * https://oeis.org/A293870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_substring_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – the nth integer containing the decimal substring "10".
   The implementation performs a linear scan from 10 upward, counting
   numbers whose base‑10 representation contains the pattern "10".
   For each candidate it examines the digits from most significant to
   least significant using modulo 10 and division.  This is O(L*M) where
   L is the number of candidates examined and M the number of digits in
   each candidate; for the typical n up to 10 000 the running time is
   well under 2 s on modern hardware. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_substring_10.h"

seq_t a(idx_t n)
{
    seq_t num = (seq_t)10;   /* the first number that can contain "10" */
    idx_t count = 0;

    while (1) {
        if (has_substring_10(num)) {
            ++count;
            if (count == n) {
                return num;
            }
        }
        ++num;                   /* advance to next integer */
    }
}

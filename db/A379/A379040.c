/*
 * A379040  Fixed points in A379015.
 * https://oeis.org/A379040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer whose non‑adjacent form (NAF) representation
 *            is a palindrome. Computed by testing successive integers
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   naf_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/naf_palindrome.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;  /* a(1) = 0 */

    idx_t count = 1;              /* already counted 0 */
    seq_t m = 1;                  /* start testing from 1 */

    for (;;) {
        if (naf_palindrome(m)) {
            ++count;
            if (count == n) return m;
        }
        ++m;
    }
}

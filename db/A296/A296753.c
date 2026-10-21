/*
 * A296753  Numbers whose base-14 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_base14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A296753 by brute‑force enumeration
 * of base‑14 digits: numbers whose number of rises equals number of falls.
 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/good_base14.h"

/* Main sequence generator: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n = 0; return 0 as a safe default */
        return 0;

    seq_t candidate = 1;
    idx_t count = 0;

    while (true) {
        if (good_base14(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}

/*
 * A381530  a(n) is the least k > 0 such that n / k contains a digit 1 in its
 *          decimal representation.
 * https://oeis.org/A381530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   contains_one_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least positive integer k such that n/k contains the digit 1
   in its infinite decimal expansion. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/contains_one_idx.h"

seq_t a(idx_t n) {
    /* search for the smallest k>0 */
    static unsigned char *seen = NULL;
    static idx_t seen_cap = 0;
    for (idx_t k = 1; ; ++k) {
        /* integer part check */
        idx_t int_part = n / k;
        if (contains_one_idx(int_part)) return (seq_t)k;

        /* fractional part */
        idx_t r = n % k;
        if (r == 0) continue;           /* terminating without more digits */

        /* ensure seen array large enough */
        if (seen_cap < k) {
            free(seen);
            seen = (unsigned char *)malloc(k);
            seen_cap = k;
        }
        memset(seen, 0, k);

        while (1) {
            r *= 10;
            idx_t digit = r / k;
            if (digit == 1) return (seq_t)k;
            r %= k;
            if (r == 0) break;          /* terminating */
            if (seen[r]) break;         /* cycle detected */
            seen[r] = 1;
        }
    }
}

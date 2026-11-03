/*
 * A336338  Numbers k such that gcd(k, composite(k)) is even.
 * https://oeis.org/A336338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   gcd, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that gcd(k, composite(k)) is even. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  // offset starts at 1, this is a safe guard
    idx_t found = 0;
    seq_t current = 4;  /* first composite number */

    for (idx_t k = 1; ; ++k) {
        /* Check if gcd(k, composite(k)) is even */
        seq_t g = gcd((seq_t)k, current);
        if ((g % 2) == 0) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }

        /* Advance to the next composite number */
        seq_t next = current + 1;
        while (next < 4 || is_prime_seq(next)) {
            ++next;
        }
        current = next;
    }
}

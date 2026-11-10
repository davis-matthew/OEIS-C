/*
 * A375508  Begin A160649 with n instead of 2; a(n) is the position in the new
 *          sequence at which it generates the same numbers as A160649 or
 *          a(n)=0 if it doesn't.
 * https://oeis.org/A375508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let b(1)=n+1, b(k+1)=b(k)+A001222(b(k)); a(n) = least k such
 *            that b(k) is a term of A160649 (A160649: c(1)=2,
 *            c(j+1)=c(j)+A001222(c(j))), else 0. Computed by a two-pointer
 *            merge of two increasing sequences
 * Range:     u8 n <= 786, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9374 (2026-11-10)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega_seq.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n + 1;   /* new sequence, starts at n+1 */
    seq_t y = (seq_t)2;       /* A160649 */
    idx_t pos = 1;
    unsigned long guard = 0;
    for (;;) {
        while (y < x) {
            y += bigomega_seq(y);
            if (++guard > 200000000UL) return (seq_t)0;
        }
        if (y == x) return (seq_t)pos;
        x += bigomega_seq(x);
        pos++;
        if (++guard > 200000000UL) return (seq_t)0;
    }
}

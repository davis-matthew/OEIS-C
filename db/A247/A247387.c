/*
 * A247387  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (1234567)*.
 * https://oeis.org/A247387
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A247387: Base-n state complexity of partitioned DFA for the periodic sequence (1234567)*.
   The sequence is periodic with period 7 starting from index 2:
   a(2)=21, a(3)=42, a(4)=21, a(5)=42, a(6)=14, a(7)=8, a(8)=7, and repeats.
   In general, for n >= 2,
        if n % 7 == 0 → 8
        if n % 7 == 1 → 7
        if n % 7 == 2 → 21
        if n % 7 == 3 → 42
        if n % 7 == 4 → 21
        if n % 7 == 5 → 42
        if n % 7 == 6 → 14
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is undefined; return 0. */
    if (n < 2)
        return (seq_t)0;

    switch (n % 7) {
    case 0: return (seq_t)8;  /* n % 7 == 0 → a(n) = 8   (n = 7,14,21,...) */
    case 1: return (seq_t)7;  /* n % 7 == 1 → a(n) = 7   (n = 8,15,22,...) */
    case 2: return (seq_t)21; /* n % 7 == 2 → a(n) = 21  (n = 2,9,16,...) */
    case 3: return (seq_t)42; /* n % 7 == 3 → a(n) = 42  (n = 3,10,17,...) */
    case 4: return (seq_t)21; /* n % 7 == 4 → a(n) = 21  (n = 4,11,18,...) */
    case 5: return (seq_t)42; /* n % 7 == 5 → a(n) = 42  (n = 5,12,19,...) */
    case 6: return (seq_t)14; /* n % 7 == 6 → a(n) = 14  (n = 6,13,20,...) */
    default: return (seq_t)0; /* unreachable, but keep compiler happy */
    }
}

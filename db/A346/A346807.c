/*
 * A346807  Numbers that are the sum of ten squares in eight or more ways.
 * https://oeis.org/A346807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 64 for n >= 8 (conjectured)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 8 the values are: 58, 61, 64, 66, 67, 69, 70. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 7) {
        switch (n) {
            case 1: return (seq_t)58;
            case 2: return (seq_t)61;
            case 3: return (seq_t)64;
            case 4: return (seq_t)66;
            case 5: return (seq_t)67;
            case 6: return (seq_t)69;
            case 7: return (seq_t)70;
            default: return (seq_t)0;    /* unreachable for valid n >= 1 */
        }
    }
    /* For n >= 8 use the conjectured formula */
    return (seq_t)n + (seq_t)64;
}

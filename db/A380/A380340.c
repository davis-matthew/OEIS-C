/*
 * A380340  a(n) = phi(1 + phi(2 + phi(3 + ... phi(n)))).
 * https://oeis.org/A380340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n = 1,2); 2 (n = 3,4); 4 (n >= 5)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    if (n <= 4) {
        switch ((idx_t)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)1;
            case 3: return (seq_t)2;
            case 4: return (seq_t)2;
            default: return 0; /* unreachable */
        }
    }
    return (seq_t)4;
}

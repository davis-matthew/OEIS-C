/*
 * A298952  First put a(n)=0 for all n, then start with a(0) = 1 and add at
 *          step n >= 0 the term 1 at position 2*n + a(n).
 * https://oeis.org/A298952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A010060(n) XOR (floor(log_2(n)) mod 2) for n ≥ 1; a(0) =
 *            1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* parity of popcount(n) */
    int popparity = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp &= tmp - 1;   /* remove lowest set bit */
        popparity ^= 1;   /* toggle parity */
    }

    /* parity of floor(log2(n)) (i.e. the number of shifts needed to reduce n to 1) */
    int logparity = 0;
    tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        logparity ^= 1;   /* toggle parity each shift */
    }

    return (seq_t)(popparity ^ logparity); /* XOR the two parities */
}

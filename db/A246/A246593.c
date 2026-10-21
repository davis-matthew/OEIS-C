/*
 * A246593  a(n)=n for n <= 2; for n >= 3, a(n) = largest number that can be
 *          obtained by swapping two bits in the binary expansion of n.
 * https://oeis.org/A246593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n except when a 0 bit exists in a more significant
 *            position than a 1 bit; then swap the most‑significant 0 with the
 *            least‑significant 1 to obtain the largest possible value
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small arguments are trivial. */
    if (n <= 2u) return (seq_t)n;

    seq_t x = (seq_t)n;

    /* Find the index of the least‑significant 1 (rightmost 1). */
    int last1 = -1;
    seq_t tmp = x;
    int i = 0;
    while ((tmp & (seq_t)1) == 0) {
        tmp >>= 1;
        ++i;
    }
    last1 = i;            /* guaranteed to exist because n > 2 */

    /* Find the index of the most‑significant bit (msb). */
    int msb = -1;
    tmp = x;
    while (tmp) {
        ++msb;
        tmp >>= 1;
    }

    /* Find the most‑significant 0 bit that comes after the msb. */
    int first0 = -1;
    for (int j = msb - 1; j >= 0; --j) {
        if (((x >> j) & (seq_t)1) == 0) {
            first0 = j;
            break;
        }
    }

    /* If there is no such zero, or it is not more significant than the
       rightmost 1, the swap would not increase the value; return n. */
    if (first0 == -1 || first0 <= last1)
        return x;

    /* Perform the swap by setting the zero to 1 and clearing the 1 to 0. */
    seq_t res = x | ((seq_t)1 << first0);   /* set the former 0 to 1 */
    res &= ~(((seq_t)1 << last1));          /* clear the former 1 to 0 */

    return res;
}

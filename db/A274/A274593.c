/*
 * A274593  a(0) = 0; thereafter, a(2*n+1) = a(n)+2*n+1, otherwise a(n) = n.
 * https://oeis.org/A274593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A274593
 *  Recurrence:
 *      a(0)      = 0
 *      a(2*n)    = 2*n
 *      a(2*n+1)  = a(n) + (2*n+1)
 *
 *  The function below computes a(n) iteratively, following the recurrence
 *  directly.  It works for any idx_t/seq_t combination supported by the
 *  build (unsigned 32/64/128‑bit or arbitrary‑precision integer).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t cur = n;

    /* Handle n = 0 in one shot */
    while (cur > 0) {
        sum += (seq_t)cur;             /* add current term (odd or even)   */
        if ((cur & 1ULL) == 0)         /* even: finished, a(cur) = cur     */
            break;
        cur = (cur - 1) >> 1;          /* move to parent: (cur-1)/2        */
    }
    return sum;
}

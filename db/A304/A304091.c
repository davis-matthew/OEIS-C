/*
 * A304091  a(n) is the number of the proper divisors of n that are Lucas
 *          numbers (A000032, with 2 included).
 * https://oeis.org/A304091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors of n that are Lucas numbers
 *            (including 2)
 * Range:     u8 n <= 105, u16 n <= 9911, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is computed by iterating the Lucas sequence up to < n and counting how many of them divide n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    seq_t cnt = 0;

    /* Lucas numbers: L0 = 2, L1 = 1 */
    if (2 < n && n % (idx_t)2 == 0) cnt++;   /* L0 */
    if (1 < n && n % (idx_t)1 == 0) cnt++;   /* L1 (always true for n>1) */

    seq_t l_prev = 2; /* L0 */
    seq_t l_cur  = 1; /* L1 */

    for (;;) {
        seq_t l_next = l_prev + l_cur; /* Lk = Lk-1 + Lk-2 */
        if (l_next >= n) {
            break;
        }
        if (n % (idx_t)l_next == 0) {
            cnt++;
        }
        l_prev = l_cur;
        l_cur = l_next;
    }

    return cnt;
}

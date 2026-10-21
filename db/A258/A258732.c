/*
 * A258732  Number of length n+2 0..3 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A258732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/5040*n^7 + 1/120*n^6 + 53/360*n^5 + 5/4*n^4 +
 *            5167/720*n^3 + 2489/120*n^2 + 2591/105*n + 10 The expression
 *            equals (n^7 + 42n^6 + 742n^5 + 6300n^4 + 36169n^3 + 104538n^2 +
 *            124368n + 50400) / 5040, a degree‑7 polynomial
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 74, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use unsigned __int128 for intermediate arithmetic; safe for n up to about 10⁴
       and compatible with 128‑bit or arbitrary‑precision seq_t.  If seq_t itself is
       128‑bit, casting back preserves the value. */
    unsigned __int128 x = n;
    unsigned __int128 n1 = x;               /* n^1   */
    unsigned __int128 n2 = n1 * n1;         /* n^2   */
    unsigned __int128 n3 = n2 * n1;         /* n^3   */
    unsigned __int128 n4 = n3 * n1;         /* n^4   */
    unsigned __int128 n5 = n4 * n1;         /* n^5   */
    unsigned __int128 n6 = n5 * n1;         /* n^6   */
    unsigned __int128 n7 = n6 * n1;         /* n^7   */

    unsigned __int128 num =
          n7
        + (unsigned __int128)42  * n6
        + (unsigned __int128)742  * n5
        + (unsigned __int128)6300 * n4
        + (unsigned __int128)36169 * n3
        + (unsigned __int128)104538 * n2
        + (unsigned __int128)124368 * n1
        + (unsigned __int128)50400;

    return (seq_t)(num / 5040);
}

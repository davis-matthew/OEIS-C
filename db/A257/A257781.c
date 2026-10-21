/*
 * A257781  Positive integers whose square is the sum of 50 consecutive
 *          squares.
 * https://oeis.org/A257781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 57, u64 n <= 133, u128 n <= 283,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257781: Positive integers whose square is the sum of 50 consecutive squares.
   Recurrence: a(n) = 6*a(n-6) - a(n-12)   (n ≥ 13)
   Initial values (offset 1):
      a(1)=245, a(2)=385, a(3)=495, a(4)=655, a(5)=795, a(6)=1055,
      a(7)=1365, a(8)=2205, a(9)=2855, a(10)=3795, a(11)=4615, a(12)=6135
*/
#include <stddef.h>

/* Wrapper preserving external linkage */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined, return dummy */

    /* Handle the first 12 terms directly */
    static const seq_t init[12] = {
        (seq_t)245, (seq_t)385, (seq_t)495, (seq_t)655,
        (seq_t)795, (seq_t)1055, (seq_t)1365, (seq_t)2205,
        (seq_t)2855, (seq_t)3795, (seq_t)4615, (seq_t)6135
    };
    if (n <= 12)
        return init[n - 1];

    /* Compute successive terms using a 12‑slot circular buffer.
       buf[k] holds the value a(k+1), where indices are taken modulo 12. */
    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    seq_t val;
    for (idx_t i = 13; i <= n; ++i) {
        /* Positions of the needed previous terms:
           a(i-6) stored at buf[(i-6-1)%12]
           a(i-12) stored at buf[(i-12-1)%12]  */
        idx_t pos_i   = (i - 1) % 12;            /* where a(i) will be stored */
        idx_t pos_i6  = (i - 6 - 1) % 12;        /* a(i-6) */
        idx_t pos_i12 = (i - 12 - 1) % 12;       /* a(i-12) */

        val = (seq_t)6 * buf[pos_i6] - buf[pos_i12];
        buf[pos_i] = val;
    }

    return buf[(n - 1) % 12];
}

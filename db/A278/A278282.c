/*
 * A278282  Number of 3Xn 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) or (1,0), with upper left element zero.
 * https://oeis.org/A278282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(n) = 3*a(n-1) + 14*a(n-2) + 3*a(n-3) - a(n-4) - 20*a(n-5) - 65*a(n-6)
          + 20*a(n-7) + 53*a(n-8) + 10*a(n-9) + 55*a(n-10) + 12*a(n-11)
          - 3*a(n-12) + 13*a(n-13) - 8*a(n-14) + 3*a(n-15) - a(n-16),
   for n > 16.  
   Initial values: a(1)=0, a(2)=5, a(3)=25, a(4)=122, a(5)=661, a(6)=3723,
   a(7)=20736, a(8)=115446, a(9)=643201, a(10)=3583987, a(11)=19968771,
   a(12)=111259772, a(13)=619906272, a(14)=3453935575, a(15)=19244300244,
   a(16)=107223525884. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial 16 terms */
    static const seq_t init[16] = {
        (seq_t)0ULL,
        (seq_t)5ULL,
        (seq_t)25ULL,
        (seq_t)122ULL,
        (seq_t)661ULL,
        (seq_t)3723ULL,
        (seq_t)20736ULL,
        (seq_t)115446ULL,
        (seq_t)643201ULL,
        (seq_t)3583987ULL,
        (seq_t)19968771ULL,
        (seq_t)111259772ULL,
        (seq_t)619906272ULL,
        (seq_t)3453935575U,
        (seq_t)19244300244ULL,
        (seq_t)107223525884ULL
    };

    if (n <= 16)
        return init[n - 1];

    /* slide a window of the last 16 values */
    seq_t terms[16];
    for (int k = 0; k < 16; ++k)
        terms[k] = init[k];

    /* number of recurrence steps needed */
    size_t steps = (size_t)(n - 16);
    for (size_t s = 0; s < steps; ++s) {
        seq_t a_n =
            (seq_t)3 * terms[15] + (seq_t)14 * terms[14] + (seq_t)3 * terms[13]
            - terms[12] - (seq_t)20 * terms[11] - (seq_t)65 * terms[10]
            + (seq_t)20 * terms[9] + (seq_t)53 * terms[8] + (seq_t)10 * terms[7]
            + (seq_t)55 * terms[6] + (seq_t)12 * terms[5] - (seq_t)3 * terms[4]
            + (seq_t)13 * terms[3] - (seq_t)8 * terms[2] + (seq_t)3 * terms[1]
            - terms[0];

        /* shift left: drop oldest, append newest */
        for (int k = 0; k < 15; ++k)
            terms[k] = terms[k + 1];
        terms[15] = a_n;
    }

    return terms[15];
}

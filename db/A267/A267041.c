/*
 * A267041  Binary representation of the n-th iteration of the "Rule 91"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10101*a(n-2) - 1010100*a(n-4) + 1000000*a(n-6) (for n>8)
 *            Initial values: a(0)=1, a(1)=101, a(2)=1010, a(3)=1100011,
 *            a(4)=1111100, a(5)=11110001111, a(6)=111110000,
 *            a(7)=111111000111111, a(8)=11111000000
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init[9] = {
    (seq_t)1,                    /* a(0) */
    (seq_t)101,                  /* a(1) */
    (seq_t)1010,                 /* a(2) */
    (seq_t)1100011,              /* a(3) */
    (seq_t)1111100,              /* a(4) */
    (seq_t)11110001111,          /* a(5) */
    (seq_t)111110000,            /* a(6) */
    (seq_t)111111000111111,      /* a(7) */
    (seq_t)11111000000           /* a(8) */
};

seq_t a(idx_t n)
{
    if (n <= 8) {
        return init[n];
    }

    /* Circular buffer of size 9 to keep the last 6 computed values
       plus the instant value at each step.  This is enough because
       the recurrence needs up to a(n-6). */
    seq_t buf[9];
    /* initialise the first 9 entries */
    for (idx_t i = 0; i < 9; ++i) {
        buf[i] = init[i];
    }

    /* compute from k = 9 up to n */
    for (idx_t k = 9; k <= n; ++k) {
        idx_t idx  =  k      % 9;   /* position to store new a(k)      */
        idx_t idx2 =  (k-2)  % 9;   /* a(k-2)                         */
        idx_t idx4 =  (k-4)  % 9;   /* a(k-4)                         */
        idx_t idx6 =  (k-6)  % 9;   /* a(k-6)                         */

        buf[idx] = (seq_t)10101 * buf[idx2]
                 - (seq_t)1010100 * buf[idx4]
                 + (seq_t)1000000 * buf[idx6];
    }

    return buf[n % 9];
}

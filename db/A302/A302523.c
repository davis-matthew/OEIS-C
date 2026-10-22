/*
 * A302523  Number of nX3 0..1 arrays with every element equal to 1, 2, 4 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A302523
 *
 *  Recurrence: a(n) = 2*a(n-1) + 7*a(n-2) + 34*a(n-3) - 44*a(n-4)
 *               - 182*a(n-5) - a(n-6) + 54*a(n-7) + 479*a(n-8)
 *               - 772*a(n-9) + 328*a(n-10) + 2176*a(n-11)
 *               - 1456*a(n-12) + 1088*a(n-13) - 4480*a(n-14)
 *               + 5120*a(n-15) - 2304*a(n-16) - 5120*a(n-17) + 4096*a(n-18)
 *
 *  Initial values (a(1) … a(18)):
 *      1, 7, 28, 115, 497, 2086, 9091, 40575, 172996, 760155,
 *      3348129, 14520438, 63606819, 278902951, 1216232548,
 *      5321761651, 23294489377, 101772095142
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 53 64-bit initial terms would overflow; only 18 are needed. */
    static const seq_t init[18] = {
        (seq_t)1ULL,
        (seq_t)7ULL,
        (seq_t)28ULL,
        (seq_t)115ULL,
        (seq_t)497ULL,
        (seq_t)2086ULL,
        (seq_t)9091ULL,
        (seq_t)40575ULL,
        (seq_t)172996ULL,
        (seq_t)760155ULL,
        (seq_t)3348129ULL,
        (seq_t)14520438ULL,
        (seq_t)63606819ULL,
        (seq_t)278902951ULL,
        (seq_t)1216232548ULL,
        (seq_t)5321761651ULL,
        (seq_t)23294489377ULL,
        (seq_t)101772095142ULL
    };

    if (n == 0) {
        /* Undefined before offset; return 0 for safety. */
        return (seq_t)0;
    }
    if (n <= 18) {
        return init[n-1];
    }

    /* Sliding window of the last 18 terms.
       state[0] = a(i-18), ..., state[17] = a(i-1)   for current i.    */
    seq_t state[18];
    for (int k = 0; k < 18; ++k) {
        state[k] = init[k];
    }

    for (idx_t i = 19; i <= n; ++i) {
        seq_t newv = 0;

        newv += (seq_t)2 * state[17];
        newv += (seq_t)7 * state[16];
        newv += (seq_t)34 * state[15];
        newv -= (seq_t)44 * state[14];
        newv -= (seq_t)182 * state[13];
        newv -= state[12];
        newv += (seq_t)54 * state[11];
        newv += (seq_t)479 * state[10];
        newv -= (seq_t)772 * state[9];
        newv += (seq_t)328 * state[8];
        newv += (seq_t)2176 * state[7];
        newv -= (seq_t)1456 * state[6];
        newv += (seq_t)1088 * state[5];
        newv -= (seq_t)4480 * state[4];
        newv += (seq_t)5120 * state[3];
        newv -= (seq_t)2304 * state[2];
        newv -= (seq_t)5120 * state[1];
        newv += (seq_t)4096 * state[0];

        /* Shift window left and store new term at the end. */
        for (int k = 0; k < 17; ++k) {
            state[k] = state[k+1];
        }
        state[17] = newv;
    }

    return state[17];
}

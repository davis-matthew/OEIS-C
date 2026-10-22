/*
 * A303520  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          5 or 6 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n‑1) − 3*a(n‑2) +10*a(n‑3) −22*a(n‑4)+6*a(n‑5)
 *            −31*a(n‑6) +10*a(n‑7) −3*a(n‑8) −14*a(n‑9) −8*a(n‑10)
 *            −32*a(n‑11) −16*a(n‑12) for n > 15 with initial values a(1)=3,
 *            a(2)=11, a(3)=18, a(4)=39, a(5)=95, a(6)=246, a(7)=687,
 *            a(8)=2023, a(9)=6126, a(10)=19571, a(11)=63599, a(12)=209618,
 *            a(13...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[15] = {
    (seq_t)3,     (seq_t)11,    (seq_t)18,   (seq_t)39,
    (seq_t)95,    (seq_t)246,   (seq_t)687,  (seq_t)2023,
    (seq_t)6126,  (seq_t)19571, (seq_t)63599, (seq_t)209618,
    (seq_t)700539,(seq_t)2356511,(seq_t)7961506
};

seq_t a(idx_t n)
{
    if (n <= 15) {
        return init[n - 1];
    }

    /* Buffer holds a(i‑12) … a(i‑1).  Initially i = 16. */
    seq_t buf[12];
    buf[0] = init[3];   /* a(4)  */
    buf[1] = init[4];   /* a(5)  */
    buf[2] = init[5];   /* a(6)  */
    buf[3] = init[6];   /* a(7)  */
    buf[4] = init[7];   /* a(8)  */
    buf[5] = init[8];   /* a(9)  */
    buf[6] = init[9];   /* a(10) */
    buf[7] = init[10];  /* a(11) */
    buf[8] = init[11];  /* a(12) */
    buf[9] = init[12];  /* a(13) */
    buf[10] = init[13]; /* a(14) */
    buf[11] = init[14]; /* a(15) */

    seq_t val;           /* a(i) being computed */

    for (idx_t i = 16; i <= n; ++i) {
        val =  4 * buf[11]
             - 3 * buf[10]
             +10 * buf[9]
             -22 * buf[8]
              +6 * buf[7]
             -31 * buf[6]
              +10 * buf[5]
             - 3 * buf[4]
             -14 * buf[3]
             - 8 * buf[2]
             -32 * buf[1]
             -16 * buf[0];

        /* Shift buffer left, drop a(i‑12), keep a(i‑11)…a(i‑1), push a(i). */
        for (int j = 0; j < 11; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[11] = val;
    }

    return val;
}

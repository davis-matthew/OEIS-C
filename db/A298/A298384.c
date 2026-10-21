/*
 * A298384  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-3) - 17*a(n-4) - 25*a(n-5) + 23*a(n-6) +
 *            33*a(n-7) + 41*a(n-8) - 145*a(n-9) - 147*a(n-10) + 24*a(n-11) +
 *            384*a(n-12) + 267*a(n-13) - 72*a(n-14) - 207*a(n-15) -
 *            61*a(n-16) + 51*a(n-17) + 10*a(n-18) - 14*a(n-19) - 4*a(n-20)
 *            for n > 21. Initial values a(1) … a(21): 1,18,56,22...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[21] = {
    (seq_t)1,  (seq_t)18,  (seq_t)56,  (seq_t)223,  (seq_t)849,
    (seq_t)3387, (seq_t)13075, (seq_t)51006,   (seq_t)199243,
    (seq_t)777845, (seq_t)3035261, (seq_t)11848853,
    (seq_t)46252538, (seq_t)180542983, (seq_t)704739373,
    (seq_t)2750937235, (seq_t)10738195965, (seq_t)41916200522,
    (seq_t)163618636603, (seq_t)638680511133, (seq_t)2493070208921
};

seq_t a(idx_t n)
{
    /* n <= 0 is not part of the sequence; return 0 safely */
    if (n == 0) return (seq_t)0;

    if (n <= 21) return init_vals[n-1];

    /* buffer holds the last 20 terms: buf[1] = a(n-1), …, buf[20] = a(n-20) */
    seq_t buf[21] = { 0 }; /* unused element buf[0] */

    /* initialise buffer for n = 22 */
    buf[1]  = init_vals[20]; /* a(21) */
    buf[2]  = init_vals[19]; /* a(20) */
    buf[3]  = init_vals[18]; /* a(19) */
    buf[4]  = init_vals[17]; /* a(18) */
    buf[5]  = init_vals[16]; /* a(17) */
    buf[6]  = init_vals[15]; /* a(16) */
    buf[7]  = init_vals[14]; /* a(15) */
    buf[8]  = init_vals[13]; /* a(14) */
    buf[9]  = init_vals[12]; /* a(13) */
    buf[10] = init_vals[11]; /* a(12) */
    buf[11] = init_vals[10]; /* a(11) */
    buf[12] = init_vals[9];  /* a(10) */
    buf[13] = init_vals[8];  /* a(9) */
    buf[14] = init_vals[7];  /* a(8) */
    buf[15] = init_vals[6];  /* a(7) */
    buf[16] = init_vals[5];  /* a(6) */
    buf[17] = init_vals[4];  /* a(5) */
    buf[18] = init_vals[3];  /* a(4) */
    buf[19] = init_vals[2];  /* a(3) */
    buf[20] = init_vals[1];  /* a(2) */

    seq_t new_val;
    for (idx_t cur = 22; cur <= n; ++cur) {
        new_val = 0;
        new_val += (seq_t)4   * buf[1];
        new_val += (seq_t)4   * buf[3];
        new_val -= (seq_t)17  * buf[4];
        new_val -= (seq_t)25  * buf[5];
        new_val += (seq_t)23  * buf[6];
        new_val += (seq_t)33  * buf[7];
        new_val += (seq_t)41  * buf[8];
        new_val -= (seq_t)145 * buf[9];
        new_val -= (seq_t)147 * buf[10];
        new_val += (seq_t)24  * buf[11];
        new_val += (seq_t)384 * buf[12];
        new_val += (seq_t)267 * buf[13];
        new_val -= (seq_t)72  * buf[14];
        new_val -= (seq_t)207 * buf[15];
        new_val -= (seq_t)61  * buf[16];
        new_val += (seq_t)51  * buf[17];
        new_val += (seq_t)10  * buf[18];
        new_val -= (seq_t)14  * buf[19];
        new_val -= (seq_t)4   * buf[20];

        /* shift buffer one step: buf[i] = previous buf[i-1] */
        for (int i = 20; i >= 2; --i) buf[i] = buf[i-1];
        buf[1] = new_val;
    }
    /* buf[1] now holds a(n) */
    return buf[1];
}

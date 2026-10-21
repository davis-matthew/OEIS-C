/*
 * A296381  Number of n X 3 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A296381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A296381: Number of n×3 0..1 arrays with each 1 horizontally, vertically or antidiagonally
 * adjacent to 1 or 2 neighboring 1s.
 *
 * Linear recurrence (empirical) of order 16:
 *   a(n) = 5*a(n-1) - 2*a(n-2) + 14*a(n-3) - 20*a(n-4) + 24*a(n-5) - 52*a(n-6)
 *          + 83*a(n-7) - 82*a(n-8) + 43*a(n-9) - 5*a(n-10) - 4*a(n-11) - 9*a(n-12)
 *          + 19*a(n-13) - 15*a(n-14) + 6*a(n-15) - a(n-16)
 * initial terms: a(1)=4, a(2)=28, a(3)=127, a(4)=641, a(5)=3237,
 *                a(6)=16248, a(7)=81661, a(8)=410199, a(9)=2061212,
 *                a(10)=10356724, a(11)=52036349, a(12)=261457677,
 *                a(13)=1313693921, a(14)=6600648217, a(15)=33164941657,
 *                a(16)=166637153693
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial 16 terms */
    static const seq_t init[16] = {
        (seq_t)4,          (seq_t)28,          (seq_t)127,
        (seq_t)641,        (seq_t)3237,        (seq_t)16248,
        (seq_t)81661,      (seq_t)410199,      (seq_t)2061212,
        (seq_t)10356724,   (seq_t)52036349,    (seq_t)261457677,
        (seq_t)1313693921, (seq_t)6600648217,  (seq_t)33164941657,
        (seq_t)166637153693
    };

    if (n <= 16) return init[n-1];   /* offset 1 */

    /* allocate buffer for values up to n */
    /* use idx_t for indexing to avoid signed conversion pitfalls */
    seq_t *buf = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!buf) return 0;   /* allocation failure – unlikely */

    /* store the initial values */
    for (idx_t i = 1; i <= 16; ++i)
        buf[i] = init[i-1];

    /* compute using the recurrence */
    for (idx_t i = 17; i <= n; ++i) {
        seq_t r = 0;

        r += (seq_t)5  * buf[i-1];
        r -= (seq_t)2  * buf[i-2];
        r += (seq_t)14 * buf[i-3];
        r -= (seq_t)20 * buf[i-4];
        r += (seq_t)24 * buf[i-5];
        r -= (seq_t)52 * buf[i-6];
        r += (seq_t)83 * buf[i-7];
        r -= (seq_t)82 * buf[i-8];
        r += (seq_t)43 * buf[i-9];
        r -= (seq_t)5  * buf[i-10];
        r -= (seq_t)4  * buf[i-11];
        r -= (seq_t)9  * buf[i-12];
        r += (seq_t)19 * buf[i-13];
        r -= (seq_t)15 * buf[i-14];
        r += (seq_t)6  * buf[i-15];
        r -=           buf[i-16];

        buf[i] = r;
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}

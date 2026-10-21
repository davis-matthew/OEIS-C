/*
 * A281395  Number of nX3 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 48, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A281395
   Recurrence for n > 19:
       a(n) = 4*a(n-1) -4*a(n-2) +6*a(n-3) -18*a(n-4) +4*a(n-5)
              +15*a(n-6) -6*a(n-7) +31*a(n-8) -48*a(n-9) +32*a(n-10)
              +8*a(n-11) -48*a(n-12) +32*a(n-13) -16*a(n-14)
   Initial values:
       a(1)=0, a(2)=10, a(3)=131, a(4)=438, a(5)=1413,
       a(6)=3838, a(7)=10226, a(8)=26468, a(9)=66952,
       a(10)=166702, a(11)=409856, a(12)=997078, a(13)=2406401,
       a(14)=5765066, a(15)=13727694, a(16)=32525520,
       a(17)=76710590, a(18)=180193960, a(19)=421829583,
       a(20)=984396802
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial terms, index 0 unused */
    static const seq_t init[21] = {
        (seq_t)0,           /* dummy for index 0 */
        (seq_t)0,           /* a(1) */
        (seq_t)10,          /* a(2) */
        (seq_t)131,         /* a(3) */
        (seq_t)438,         /* a(4) */
        (seq_t)1413,        /* a(5) */
        (seq_t)3838,        /* a(6) */
        (seq_t)10226,       /* a(7) */
        (seq_t)26468,       /* a(8) */
        (seq_t)66952,       /* a(9) */
        (seq_t)166702,      /* a(10) */
        (seq_t)409856,      /* a(11) */
        (seq_t)997078,      /* a(12) */
        (seq_t)2406401,     /* a(13) */
        (seq_t)5765066,     /* a(14) */
        (seq_t)13727694,    /* a(15) */
        (seq_t)32525520,    /* a(16) */
        (seq_t)76710590,    /* a(17) */
        (seq_t)180193960,   /* a(18) */
        (seq_t)421829583,   /* a(19) */
        (seq_t)984396802    /* a(20) */
    };

    if (n <= 20) {
        return init[n];
    }

    /* sliding window of the last 14 terms: a(i-14) .. a(i-1) */
    seq_t prev[14];
    size_t k;
    for (k = 0; k < 14; ++k) {          /* initialise with a(7)..a(20) */
        prev[k] = init[7 + k];
    }

    seq_t val = 0;
    for (idx_t i = 21; i <= n; ++i) {
        /* compute a(i) from the 14 previous terms */
        seq_t pos = 0;
        seq_t neg = 0;

        pos += 4  * prev[13];          /* +4 * a(i-1)   */
        pos += 6  * prev[11];          /* +6 * a(i-3)   */
        pos += 4  * prev[9];           /* +4 * a(i-5)   */
        pos += 15 * prev[8];           /* +15 * a(i-6)  */
        pos += 31 * prev[6];           /* +31 * a(i-8)  */
        pos += 32 * prev[4];           /* +32 * a(i-10) */
        pos += 8  * prev[3];           /* +8  * a(i-11) */
        pos += 32 * prev[1];           /* +32 * a(i-13) */

        neg += 4  * prev[12];          /* -4  * a(i-2)  */
        neg += 18 * prev[10];          /* -18 * a(i-4)  */
        neg += 6  * prev[7];           /* -6  * a(i-7)  */
        neg += 48 * prev[5];           /* -48 * a(i-9)  */
        neg += 48 * prev[2];           /* -48 * a(i-12) */
        neg += 16 * prev[0];           /* -16 * a(i-14) */

        val = pos - neg;               /* final a(i)    */

        /* shift window left: drop a(i-14), keep a(i-13)..a(i-1) */
        for (k = 0; k < 13; ++k) {
            prev[k] = prev[k + 1];
        }
        prev[13] = val;                /* new element a(i) */
    }
    return val;                         /* a(n) */
}

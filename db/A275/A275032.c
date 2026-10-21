/*
 * A275032  Number of nX3 0..2 arrays with no element equal to any value at
 *          offset (0,-1) (-1,-2) or (-2,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275032: Number of n×3 0..2 arrays with no element equal to any
 * value at offset (0,-1), (-1,-2) or (-2,0).  Offset: 1
 *
 * Empirical linear recurrence for n > 17:
 *   a(n) = 3*a(n-1) +6*a(n-2) -20*a(n-3) -2*a(n-4) +46*a(n-5)
 *          -42*a(n-6) -31*a(n-7) +70*a(n-8) -5*a(n-9)
 *          -30*a(n-10) -2*a(n-11) +8*a(n-12) +5*a(n-13)
 *          -3*a(n-14) -2*a(n-15) +a(n-16)
 *
 * Initial values a(1)=2, a(2)=16, a(3)=46, a(4)=146, a(5)=450, a(6)=1428,
 * a(7)=4463, a(8)=14086, a(9)=44195, a(10)=139130, a(11)=437175,
 * a(12)=1375142, a(13)=4323125, a(14)=13594852, a(15)=42745150,
 * a(16)=134409630, a(17)=422629291.
 */

#include <stdint.h>

static const seq_t init_vals[17] = {
    (seq_t)2,          (seq_t)16,         (seq_t)46,          (seq_t)146,
    (seq_t)450,        (seq_t)1428,       (seq_t)4463,        (seq_t)14086,
    (seq_t)44195,      (seq_t)139130,     (seq_t)437175,      (seq_t)1375142,
    (seq_t)4323125,    (seq_t)13594852,   (seq_t)42745150,    (seq_t)134409630,
    (seq_t)422629291
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    if (n <= 17) return init_vals[n - 1];

    /* Circular buffer holding the last 17 terms a(i-16 .. i). */
    static seq_t buf[17];
    /* initialise buffer with the first 17 terms */
    for (idx_t i = 1; i <= 17; ++i)
        buf[i % 17] = init_vals[i - 1];

    for (idx_t i = 18; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)3  * buf[(i - 1) % 17];
        val += (seq_t)6  * buf[(i - 2) % 17];
        val -= (seq_t)20 * buf[(i - 3) % 17];
        val -= (seq_t)2  * buf[(i - 4) % 17];
        val += (seq_t)46 * buf[(i - 5) % 17];
        val -= (seq_t)42 * buf[(i - 6) % 17];
        val -= (seq_t)31 * buf[(i - 7) % 17];
        val += (seq_t)70 * buf[(i - 8) % 17];
        val -= (seq_t)5  * buf[(i - 9) % 17];
        val -= (seq_t)30 * buf[(i - 10) % 17];
        val -= (seq_t)2  * buf[(i - 11) % 17];
        val += (seq_t)8  * buf[(i - 12) % 17];
        val += (seq_t)5  * buf[(i - 13) % 17];
        val -= (seq_t)3  * buf[(i - 14) % 17];
        val -= (seq_t)2  * buf[(i - 15) % 17];
        val +=            buf[(i - 16) % 17];

        buf[i % 17] = val;
    }

    return buf[n % 17];
}

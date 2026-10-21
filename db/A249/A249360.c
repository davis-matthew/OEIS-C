/*
 * A249360  Number of length 1+6 0..n arrays with every seven consecutive
 *          terms having six times some element equal to the sum of the
 *          remaining six.
 * https://oeis.org/A249360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -11*a(n-2) +15*a(n-3) -15*a(n-4) +12*a(n-5)
 *            -9*a(n-6) +7*a(n-7) -4*a(n-8) +4*a(n-10) -7*a(n-11) +9*a(n-12)
 *            -12*a(n-13) +15*a(n-14) -15*a(n-15) +11*a(n-16) -5*a(n-17)
 *            +a(n-18) with initial values a(1)=2, a(2)=395, a(3)=2048,
 *            a(4)=9413, a(5)=30848, a(6)=84259, a(7)=198570, a(8)=4219...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 38, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* initial 18 terms (index 1 → a[0]) */
    static const seq_t init[18] = {
        (seq_t)2ULL,          (seq_t)395ULL,         (seq_t)2048ULL,
        (seq_t)9413ULL,       (seq_t)30848ULL,       (seq_t)84259ULL,
        (seq_t)198570ULL,     (seq_t)421913ULL,      (seq_t)823336ULL,
        (seq_t)1504241ULL,    (seq_t)2601366ULL,     (seq_t)4297747ULL,
        (seq_t)6831608ULL,    (seq_t)10505741ULL,   (seq_t)15697544ULL,
        (seq_t)22873679ULL,   (seq_t)32598374ULL,   (seq_t)45551245ULL
    };

    if (n <= 18)
        return init[n - 1];

    /* Circular buffer holding the last 18 values:
       buf[0] = a(k-18), buf[1] = a(k-17), … , buf[17] = a(k-1)   */
    seq_t buf[18];
    for (size_t i = 0; i < 18; ++i)
        buf[i] = init[i];

    /* compute a(k) from k = 19 up to n */
    for (idx_t k = 19; k <= n; ++k) {
        seq_t next =
            (seq_t)5  * buf[17]               /* a(k-1)   */
          - (seq_t)11 * buf[16]               /* a(k-2)   */
          + (seq_t)15 * buf[15]               /* a(k-3)   */
          - (seq_t)15 * buf[14]               /* a(k-4)   */
          + (seq_t)12 * buf[13]               /* a(k-5)   */
          - (seq_t)9  * buf[12]               /* a(k-6)   */
          + (seq_t)7  * buf[11]               /* a(k-7)   */
          - (seq_t)4  * buf[10]               /* a(k-8)   */
          + (seq_t)4  * buf[ 8]               /* a(k-10)  */
          - (seq_t)7  * buf[ 7]               /* a(k-11)  */
          + (seq_t)9  * buf[ 6]               /* a(k-12)  */
          - (seq_t)12 * buf[ 5]               /* a(k-13)  */
          + (seq_t)15 * buf[ 4]               /* a(k-14)  */
          - (seq_t)15 * buf[ 3]               /* a(k-15)  */
          + (seq_t)11 * buf[ 2]               /* a(k-16)  */
          - (seq_t)5  * buf[ 1]               /* a(k-17)  */
          +          buf[ 0];                /* a(k-18)  */

        /* shift buffer left by one, discarding buf[0], and insert next at end */
        for (size_t i = 0; i < 17; ++i)
            buf[i] = buf[i + 1];
        buf[17] = next;
    }

    return buf[17]; /* a(n) */
}

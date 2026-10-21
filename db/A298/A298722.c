/*
 * A298722  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 5*a(n-1) -5*a(n-2) -5*a(n-3) -2*a(n-4) +18*a(n-5)
 *            +14*a(n-6) * -30*a(n-7) -8*a(n-8) -22*a(n-9) +4*a(n-10)
 *            +84*a(n-11) +3*a(n-12) * -33*a(n-13) -9*a(n-14) -21*a(n-15)
 *            +4*a(n-16) +4*a(n-17) (n>18) * Initial values: * a(1)=1, a(2)=7,
 *            a(3)=5, a(4)=15, a(5)=21, a(6)=57, a(7)=119, * a(8)=285,...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* behavior below offset not required */

    /* Predefined first 18 terms */
    static const seq_t init[18] = {
        (seq_t)1, (seq_t)7, (seq_t)5, (seq_t)15, (seq_t)21,
        (seq_t)57, (seq_t)119, (seq_t)285, (seq_t)725, (seq_t)1833,
        (seq_t)4807, (seq_t)12843, (seq_t)34439, (seq_t)93327,
        (seq_t)254085, (seq_t)693267, (seq_t)1896489, (seq_t)5194309
    };

    if (n <= 18) /* direct return for the first 18 terms */
        return init[n - 1];

    /* Circular buffer holding the last 18 computed terms.
       buffer[i] corresponds to a(i+1) for i = 0..17. */
    seq_t buf[18];
    for (idx_t i = 0; i < 18; ++i)
        buf[i] = init[i];

    /* Compute terms from a(19) up to a(n) */
    for (idx_t cur = 19; cur <= n; ++cur) {
        /* Compute a(cur) using the recurrence. */
        seq_t val = 5   * buf[(cur - 2) % 18]
                  -5   * buf[(cur - 3) % 18]
                  -5   * buf[(cur - 4) % 18]
                  -2   * buf[(cur - 5) % 18]
                  +18  * buf[(cur - 6) % 18]
                  +14  * buf[(cur - 7) % 18]
                  -30  * buf[(cur - 8) % 18]
                  -8   * buf[(cur - 9) % 18]
                  -22  * buf[(cur -10) % 18]
                  +4   * buf[(cur -11) % 18]
                  +84  * buf[(cur -12) % 18]
                  +3   * buf[(cur -13) % 18]
                  -33  * buf[(cur -14) % 18]
                  -9   * buf[(cur -15) % 18]
                  -21  * buf[(cur -16) % 18]
                  +4   * buf[(cur -17) % 18]
                  +4   * buf[(cur -18) % 18];

        /* Store the newly computed term, overwriting the oldest one. */
        buf[(cur - 1) % 18] = val;
    }

    return buf[(n - 1) % 18];
}

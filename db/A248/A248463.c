/*
 * A248463  Number of length 2+2 0..n arrays with no three consecutive terms
 *          having the sum of any two elements equal to twice the third.
 * https://oeis.org/A248463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - 2*a(n-5) + 2*a(n-6) + a(n-8) -
 *            2*a(n-10) + a(n-11) (for n ≥ 12) Initial values (n = 1 … 11):
 *            10, 36, 148, 380, 862, 1652, 2956, 4860, 7642, 11400, 16488
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* initial terms 1..11 ; index 0 is unused (set to 0) */
    static const seq_t init[12] = {
        0ULL,               /* dummy for index 0 */
        (seq_t)10,
        (seq_t)36,
        (seq_t)148,
        (seq_t)380,
        (seq_t)862,
        (seq_t)1652,
        (seq_t)2956,
        (seq_t)4860,
        (seq_t)7642,
        (seq_t)11400,
        (seq_t)16488
    };

    /* return for the first 11 indices directly */
    if (n <= 11) {
        return init[n];
    }

    /* circular buffer that holds the last 11 computed terms
       indices 0 … 11 represent n mod 12                  */
    seq_t buf[12];
    for (idx_t i = 1; i <= 11; ++i) {
        buf[i % 12] = init[i];
    }

    /* compute terms a(12) … a(n) using the recurrence */
    for (idx_t i = 12; i <= n; ++i) {
        /* compute positive and negative contributions separately
           to avoid intermediate underflow on unsigned types   */
        seq_t pos = buf[(i - 1) % 12] + buf[(i - 1) % 12];
        pos += buf[(i - 6) % 12] + buf[(i - 6) % 12];
        pos += buf[(i - 8) % 12];
        pos += buf[(i - 11) % 12];

        seq_t neg = buf[(i - 3) % 12];
        neg += buf[(i - 5) % 12] + buf[(i - 5) % 12];
        neg += buf[(i - 10) % 12] + buf[(i - 10) % 12];

        buf[i % 12] = pos - neg;
    }

    return buf[n % 12];
}

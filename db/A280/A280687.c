/*
 * A280687  a(n) = n / A280686(n); n divided by its largest Fibonacci proper
 *          divisor, a(1) = 1.
 * https://oeis.org/A280687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n divided by the largest Fibonacci number < n that
 *            divides n. For n = 1 the value is defined to be 1
 * Range:     u8 n <= 255, u16 n <= 9964, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)1;

    /* generate all Fibonacci numbers less than n;
       there are very few for n <= 10000 (at most 20 or so). */
    uint64_t fibs[90];   /* enough to hold all Fibonacci numbers < 2^64 */
    int count = 0;
    fibs[count++] = 1;   /* F1 */
    fibs[count++] = 1;   /* F2 */

    while (1) {
        uint64_t next = fibs[count - 1] + fibs[count - 2];
        if (next >= n)         /* we need numbers strictly < n */
            break;
        fibs[count++] = next;
        if (count >= 90)       /* safety: should never happen for n <= 2^64 */
            break;
    }

    /* search from largest to smallest */
    for (int i = count - 1; i >= 0; --i) {
        if (n % fibs[i] == 0)
            return (seq_t)n / (seq_t)fibs[i];
    }

    /* fallback – should never be reached because 1 divides every n */
    return (seq_t)n;
}

/*
 * A361759  Sum of b(i) where the first b terms are all k digits of n,
 *          followed by Keith-like sum of the previous k digits until b(i) >=
 *          n.
 * https://oeis.org/A361759
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the sequence {b(i)} where the first t terms are
 *            the decimal digits of n (t = number of digits), and for i > t,
 *            b(i) = b(i‑1) + … + b(i‑t) until b(i) >= n. The sum includes the
 *            final term b(i) that first reaches or exceeds n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The algorithm works for all n, including n < 10.  The number of
       digits (t) is at most 5 for n <= 10000, but we use a dynamic
       allocation to handle larger n safely. */
    uint64_t tmp = (uint64_t)n;
    int t = 0;
    do { t++; tmp /= 10; } while (tmp != 0);

    /* Allocate array for the rolling window of the last t terms. */
    seq_t *window = (seq_t*)malloc((size_t)t * sizeof(seq_t));
    if (!window) return 0;  /* allocation failure, return 0 as safe value */

    /* Extract digits in order: most significant first. */
    tmp = (uint64_t)n;
    for (int i = t - 1; i >= 0; --i) {
        window[i] = (seq_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t sum = 0;
    seq_t sum_window = 0;
    for (int i = 0; i < t; ++i) {
        sum_window += window[i];
        sum += window[i];
    }

    int pos = 0;   /* position in the circular buffer to overwrite */
    while (1) {
        seq_t next = sum_window;
        if (next >= (seq_t)n) {
            sum += next;
            break;
        }
        /* Update window: remove oldest term, add new term */
        sum_window += next;
        sum_window -= window[pos];
        window[pos] = next;
        sum += next;
        pos = (pos + 1) % t;
    }

    free(window);
    return sum;
}

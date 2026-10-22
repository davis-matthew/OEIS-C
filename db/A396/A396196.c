/*
 * A396196  The n-th digit of the n-th term of the Look-and-Say sequence
 *          (A005150), padded with 0 if the term has fewer than n digits.
 * https://oeis.org/A396196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th digit of the n‑th Look‑and‑Say term, padded with
 *            0 if that term has fewer than n digits
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is at least 1 by the OEIS offset; for safety handle 0 */
    if (n == 0)
        return (seq_t)0;

    size_t sz = (size_t)n;           /* n fits in size_t for the intended limits */

    /* Allocate buffers to store up to the first n digits of each term */
    char *prev = (char *)malloc(sz + 1);
    char *curr = (char *)malloc(sz + 1);
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Term 1 */
    prev[0] = '1';
    size_t prev_len = 1;

    if (n == 1) {
        seq_t r = (seq_t)(prev[0] - '0');
        free(prev);
        free(curr);
        return r;
    }

    /* Generate terms 2 through n */
    for (idx_t step = 2; step <= n; ++step) {
        size_t curr_len = 0;
        size_t pos = 0;
        while (pos < prev_len && curr_len < sz) {
            char digit = prev[pos];
            size_t run_len = 1;
            ++pos;
            while (pos < prev_len && prev[pos] == digit) {
                ++run_len;
                ++pos;
            }

            /* Emit run length in decimal */
            char tmp[20];
            int t = 0;
            size_t rl = run_len;
            do {
                tmp[t++] = '0' + (rl % 10);
                rl /= 10;
            } while (rl);
            while (t > 0 && curr_len < sz) {
                curr[curr_len++] = tmp[--t];
            }

            /* Emit the digit */
            if (curr_len < sz)
                curr[curr_len++] = digit;
        }

        prev_len = curr_len;
        /* Swap buffers for next iteration */
        char *swap = prev;
        prev = curr;
        curr = swap;
    }

    /* Extract the requested digit or pad with 0 */
    seq_t result;
    if (prev_len < sz)
        result = (seq_t)0;
    else
        result = (seq_t)(prev[sz - 1] - '0');

    free(prev);
    free(curr);
    return result;
}

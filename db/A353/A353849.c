/*
 * A353849  Number of distinct positive run-sums of the n-th composition in
 *          standard order.
 * https://oeis.org/A353849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A353849: number of distinct positive run‑sums of the n-th
   composition in standard order.

   The mapping from n to the composition is obtained by taking the set
   of positions of 1-bits in the reversed binary expansion of n, prepending
   a 0, taking first differences, and reversing again.  Then the
   distinct sums of maximal runs of equal parts are counted.  */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Extract the positions of 1‑bits in the reversed binary expansion of n.
       positions are counted from 1 for the least significant bit. */
    idx_t pos[64];               /* at most 64 bits for uint64_t */
    size_t pos_len = 0;
    idx_t k = n;
    idx_t bit_pos = 1;
    while (k) {
        if (k & 1ULL) pos[pos_len++] = bit_pos;
        k >>= 1;
        bit_pos++;
    }

    /* Build the list of first differences after prepending 0.
       The number of differences equals the number of 1‑bits. */
    seq_t diff[64];
    size_t diff_len = pos_len;
    idx_t prev = 0;
    for (size_t i = 0; i < pos_len; ++i) {
        diff[i] = (seq_t)(pos[i] - prev);
        prev = pos[i];
    }

    /* Reverse the differences to obtain the composition. */
    seq_t comp[64];
    for (size_t i = 0; i < diff_len; ++i) {
        comp[diff_len - 1 - i] = diff[i];
    }

    /* For the empty composition (n = 0) the answer is 0. */
    if (diff_len == 0) return (seq_t)0;

    /* Count distinct sums of maximal runs of equal parts. */
    seq_t sums[64];   /* maximum possible distinct sums is 64 */
    size_t sums_len = 0;

    seq_t last = comp[0];
    size_t run_len = 1;
    for (size_t i = 1; i < diff_len; ++i) {
        if (comp[i] == last) {
            run_len++;
        } else {
            seq_t s = last * (seq_t)run_len;
            bool found = false;
            for (size_t j = 0; j < sums_len; ++j) {
                if (sums[j] == s) { found = true; break; }
            }
            if (!found) sums[sums_len++] = s;
            last = comp[i];
            run_len = 1;
        }
    }

    /* Handle the final run. */
    seq_t s = last * (seq_t)run_len;
    bool found = false;
    for (size_t j = 0; j < sums_len; ++j) {
        if (sums[j] == s) { found = true; break; }
    }
    if (!found) sums_len++;

    return (seq_t)sums_len;
}

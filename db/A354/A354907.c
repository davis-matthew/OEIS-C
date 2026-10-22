/*
 * A354907  Number of distinct sums of contiguous constant subsequences
 *          (partial runs) of the n-th composition in standard order.
 * https://oeis.org/A354907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums of contiguous constant subruns of
 *            the n‑th composition in graded reverse‑lexicographic order
 *            (A066099). The composition is obtained from n by taking the
 *            positions of 1‑bits in the binary representation of n (least
 *            significant bit first), prepending 0, taking first d...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Step 1: find positions of 1‑bit in n (lsb first), positions start at 1 */
    uint8_t pos[64];
    size_t pos_len = 0;
    idx_t temp = n;
    uint8_t bit_index = 1;  /* 1‑based position */
    while (temp != 0) {
        if (temp & 1)
            pos[pos_len++] = bit_index;
        temp >>= 1;
        ++bit_index;
    }

    /* Step 2: compute composition parts (differences) and reverse */
    uint8_t comp[64];
    size_t comp_len = pos_len;          /* number of parts equals number of 1 bits */
    uint8_t prev = 0;
    for (size_t i = 0; i < pos_len; ++i) {
        comp[i] = pos[i] - prev;
        prev = pos[i];
    }
    for (size_t i = 0; i < comp_len / 2; ++i) {
        uint8_t tmp = comp[i];
        comp[i] = comp[comp_len - 1 - i];
        comp[comp_len - 1 - i] = tmp;
    }

    /* Step 3: identify runs of equal values and accumulate distinct sums. */
    /* Maximum possible sum <= 63 * 64 = 4032, so a fixed array is safe. */
    bool seen[4097] = { false };
    seq_t count = 0;

    for (size_t i = 0; i < comp_len; ) {
        uint8_t val  = comp[i];
        size_t j = i;
        while (j < comp_len && comp[j] == val)
            ++j;
        size_t run_len = j - i;

        for (size_t k = 1; k <= run_len; ++k) {
            size_t sum = (size_t)val * k;
            if (!seen[sum]) {
                seen[sum] = true;
                ++count;
            }
        }
        i = j;
    }
    return count;
}

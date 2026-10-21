/*
 * A247802  Numbers in decimal representation with distinct digits, such that
 *          in Dutch their digits are in alphabetic order.
 * https://oeis.org/A247802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 106, u32 n <= 991, u64 n <= 992,
 *            u128 n <= 992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247802:
   Generate all subsets of decimal digits 0..9 such that in Dutch
   alphabetical order the digits appear in the same order.
   Subsets that would produce a leading zero (except the single digit 0)
   are discarded.  The resulting numbers are sorted and returned at
   the requested index (1‑based). */

#include <stdlib.h>
#include <stdint.h>

static int compare_seq(const void *v1, const void *v2)
{
    const seq_t *a = (const seq_t *)v1;
    const seq_t *b = (const seq_t *)v2;
    if (*a < *b) return -1;
    if (*b < *a) return 1;
    return 0;
}

static seq_t seq_cache[1024];
static size_t seq_len = 0;

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (seq_len == 0) {
        /* Dutch alphabetical order of digits:
           8 (acht), 1 (een), 3 (drie), 9 (negen),
           0 (nul), 2 (twee), 4 (vier), 5 (vijf),
           6 (zes), 7 (zeven) */
        const int order[10] = {8, 1, 3, 9, 0, 2, 4, 5, 6, 7};

        size_t id = 0;
        /* iterate over all non‑empty subsets of digits */
        for (int mask = 1; mask < (1 << 10); ++mask) {
            /* Skip subsets that would produce a leading zero
               (except the sole 0). */
            if ((mask & (1 << 0)) &&
                !(mask & ((1 << 8) | (1 << 1) | (1 << 3) | (1 << 9)))) {
                if (mask != (1 << 0)) continue;
            }

            seq_t num = 0;
            for (int k = 0; k < 10; ++k) {
                int d = order[k];
                if (mask & (1 << d)) {
                    num = num * 10 + d;
                }
            }
            seq_cache[id++] = num;
        }
        seq_len = id;
        /* Sort the list of numbers in ascending order */
        qsort(seq_cache, seq_len, sizeof(seq_t), compare_seq);
    }

    if (n == 0 || n > seq_len) return 0;
    return seq_cache[n - 1];
}

/*
 * A247808  Numbers in decimal representation with distinct digits, such that
 *          in Latin their digits are in alphabetic order.
 * https://oeis.org/A247808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   a(n) is the n‑th smallest integer whose decimal digits are
 *            distinct and appear in increasing order according to the Latin
 *            names of the digits (order: 2,9,8,4,5,6,7,3,1,0). The algorithm
 *            enumerates every non‑empty subset of the ten digits, builds the
 *            corresponding number by writing the digits in t...
 * Range:     u8 n <= 1, u16 n <= 175, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort that works for any seq_t size. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Digit order according to Latin alphabetic order of digit names. */
static const int digit_order[10] = {2,9,8,4,5,6,7,3,1,0};

seq_t a(idx_t n)
{
    static seq_t *list = NULL;
    static size_t len = 0;

    if (!list) {
        const size_t total = ((size_t)1) << 10;   /* 2^10 subsets of digits */
        len = total - 1;                          /* exclude empty subset */

        list = (seq_t *)malloc(len * sizeof(seq_t));
        if (!list) return (seq_t)0;               /* allocation failure */

        /* Build each number from its digit subset. */
        for (size_t mask = 1; mask < total; ++mask) {
            seq_t num = 0;
            for (int i = 0; i < 10; ++i) {
                if (mask & ((size_t)1 << i)) {
                    num = num * ((seq_t)10) + (seq_t)digit_order[i];
                }
            }
            list[mask - 1] = num;
        }

        /* Sort numerically. */
        qsort(list, len, sizeof(seq_t), cmp_seq);
    }

    if (n == 0 || (idx_t)n > len)
        return (seq_t)0;   /* out of bounds or invalid index */
    return list[n - 1];
}

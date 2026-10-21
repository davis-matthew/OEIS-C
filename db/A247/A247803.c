/*
 * A247803  Numbers in decimal representation with distinct digits, such that
 *          in Finnish their digits are in alphabetic order.
 * https://oeis.org/A247803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 165, u32 n <= 1007, u64 n <= 1008,
 *            u128 n <= 1008, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Computes A247803: numbers with distinct decimal digits that are
   alphabetically sorted in Finnish (digit order 8,2,3,6,4,0,7,5,9,1).
   The function generates all such numbers once, sorts them
   numerically, and returns the n‑th (1‑based) element.  If n is
   beyond the last term, the function returns 0. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    return (*pa < *pb) ? -1 : (*pa > *pb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static bool built = false;
    static seq_t static_arr[1024];
    static size_t static_len = 0;

    if (!built) {
        const int digit_order[10] = {8, 2, 3, 6, 4, 0, 7, 5, 9, 1};
        const int earlierMask = (1 << 8) | (1 << 2) | (1 << 3) |
                                (1 << 6) | (1 << 4);
        const int maxMask = 1 << 10;           /* 2^10 subsets (excluding empty) */
        size_t cnt = 0;

        for (int mask = 1; mask < maxMask; ++mask) {
            bool hasZero = (mask & (1 << 0)) != 0;
            if (hasZero && mask != (1 << 0) && (mask & earlierMask) == 0)
                continue;                       /* avoid leading zero */

            seq_t value = 0;
            for (int i = 0; i < 10; ++i) {
                int d = digit_order[i];
                if (mask & (1 << d)) {
                    value = value * 10 + (seq_t)d;
                }
            }
            static_arr[cnt++] = value;
        }

        static_len = cnt;
        qsort(static_arr, static_len, sizeof(seq_t), cmp_seq);
        built = true;
    }

    if (n < 1)
        return 0;
    size_t idx = (size_t)(n - 1);
    if (idx >= static_len)
        return 0;
    return static_arr[idx];
}

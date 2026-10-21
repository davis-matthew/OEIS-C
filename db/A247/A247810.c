/*
 * A247810  Numbers in decimal representation with distinct digits, such that
 *          in Polish their digits are in alphabetic order.
 * https://oeis.org/A247810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 113, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: enumerate all non‑empty subsets of {0,…,9} (2¹⁰‑1 = 1023 numbers),
   build each number by writing its digits in Polish alphabetical order
   (4,2,9,1,8,5,6,7,3,0), then sort the resulting list numerically.
   a(n) returns the nth smallest such number. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache: initialized on first call */
    static bool initialized = false;
    static seq_t *table = NULL;
    static size_t table_size = 0;

    if (!initialized) {
        /* There are 2^10-1 = 1023 non‑empty subsets of digits 0..9 */
        table_size = ((idx_t)1 << 10) - 1;
        table = (seq_t *)malloc(table_size * sizeof(seq_t));

        /* Polish alphabetical order of digits */
        const unsigned char order[10] = {4, 2, 9, 1, 8, 5, 6, 7, 3, 0};

        idx_t mask;
        size_t idx = 0;
        for (mask = 1; mask < ((idx_t)1 << 10); ++mask) {
            seq_t num = 0;
            for (int i = 0; i < 10; ++i) {
                if (mask & ((idx_t)1 << i)) {
                    num = num * 10 + order[i];
                }
            }
            table[idx++] = num;
        }

        qsort(table, table_size, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n < 1 || n > table_size) {
        /* undefined behaviour for out‑of‑range n; return 0 as sentinel */
        return 0;
    }
    return table[n - 1];
}

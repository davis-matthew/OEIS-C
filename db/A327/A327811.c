/*
 * A327811  Numbers obtained from cyclically permuting the base-7 digits of
 *          13143449029 and converting back to decimal.
 * https://oeis.org/A327811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) = the n‑th smallest value among the 12 cyclic permutations
 *            of the base-7 digits of 13143449029 when interpreted back in
 *            decimal. The sequence has 12 terms (offset 1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compilation requirement: C11 / C++17, seq_t and idx_t supplied by the build. */
#include <stdlib.h>
#include <stdbool.h>

/* Digits of 13143449029 in base 7: 643464321244. */
static const unsigned char base7_digits[12] = {6,4,3,4,6,4,3,2,1,2,4,4};

/* Comparator for ascending order. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Return a(n). */
seq_t a(idx_t n)
{
    /* Cache for the 12 sorted values. */
    static seq_t sorted[12];
    static bool ready = false;

    if (!ready) {
        seq_t vals[12];
        for (int r = 0; r < 12; ++r) {
            seq_t v = 0;
            for (int i = 0; i < 12; ++i) {
                v = v * 7 + base7_digits[(r + i) % 12];
            }
            vals[r] = v;
        }
        /* Copy into sorted array and sort ascending. */
        for (int i = 0; i < 12; ++i) sorted[i] = vals[i];
        qsort(sorted, 12, sizeof(seq_t), cmp_seq);
        ready = true;
    }

    if (n < 1 || n > 12) return 0;  /* undefined for other indices */
    return sorted[(size_t)(n - 1)];
}

/*
 * A247814  Numbers in decimal representation with distinct digits, such that
 *          in Turkish their digits are in alphabetic order.
 * https://oeis.org/A247814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 1019, u64 n <= 1020,
 *            u128 n <= 1020, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A247814: numbers with distinct decimal digits whose Turkish word names
   are in alphabetic order.  The order of the digits by their Turkish names
   is: 6,5,1,9,4,2,8,0,3,7.
   The sequence consists of all numbers obtained by taking any non‑empty
   subset of {0,…,9}, ordering its digits according to the Turkish order,
   forming the decimal number, and discarding any subset that would
   create a leading zero (i.e. subsets that contain 0 and no digit among
   6,5,1,9,4,2,8).  The remaining numbers are sorted numerically.  */

#include <stdlib.h>

/* Compare two seq_t values for qsort. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Table of digits in Turkish alphabetical order. */
static const int TURKISH_ORDER[10] = {6, 5, 1, 9, 4, 2, 8, 0, 3, 7};

/* Mask of digits that occur before the digit 0 in Turkish order.
   These digits are 6,5,1,9,4,2,8. */
#define BEFORE_ZERO_MASK  ( (1U<<6)|(1U<<5)|(1U<<1)|(1U<<9)|(1U<<4)|(1U<<2)|(1U<<8) )

/* Build the entire sequence on first use. */
static seq_t values[1020];
static int built = 0;

/* Generate all numbers according to the definition and sort them. */
static void build_sequence(void)
{
    size_t pos = 0;

    for (int mask = 1; mask < (1 << 10); ++mask) {
        /* Skip subsets that would produce a leading zero except for {0}. */
        if ((mask & 1) && ((mask & BEFORE_ZERO_MASK) == 0) && (mask != 1))
            continue;

        seq_t num = 0;
        for (int i = 0; i < 10; ++i) {
            int d = TURKISH_ORDER[i];
            if (mask & (1 << d))
                num = num * 10 + (seq_t)d;
        }
        values[pos++] = num;
    }

    /* pos should now be 1020. */
    qsort(values, pos, sizeof(seq_t), cmp_seq_t);
    built = 1;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 1020)
        return 0;  /* undefined for out-of-range indices */

    if (!built)
        build_sequence();

    return values[n - 1];
}

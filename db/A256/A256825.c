/*
 * A256825  Numbers with digits in strictly decreasing order containing digit
 *          1.
 * https://oeis.org/A256825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   the nth term is the nth smallest integer whose digits are *
 *            strictly decreasing and contain the digit 1. The sequence
 *            consists of * all numbers obtained by taking any subset of the
 *            digits {0,…,9} that * contains 1, arranging the chosen digits in
 *            descending order, * and listing the resulting numbe...
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 511, u64 n <= 512,
 *            u128 n <= 512, big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has exactly 512 terms, from 1 up to 9876543210. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_cmp.h"

/* comparison function for qsort – works for any seq_t type */

/* static cache of all 512 terms, generated on first call */
static seq_t table[512];
static bool ready = false;

static void build_table(void)
{
    /* for each subset of digits {0,2,…,9} (bits 0–8) */
    for (idx_t mask = 0; mask < (idx_t)512; ++mask)
    {
        seq_t num = 0;
        /* digits are placed in strictly decreasing order from 9 down to 0 */
        for (int d = 9; d >= 0; --d)
        {
            if (d == 1)
            {
                /* digit 1 is always included */
                num = num * 10 + 1;
            }
            else
            {
                int bit = (d == 0) ? 0 : d - 1; /* map digit to bit index */
                if (mask & (1ULL << bit))
                    num = num * 10 + (seq_t)d;
            }
        }
        table[(size_t)mask] = num;
    }
    /* sort the numbers to obtain the OEIS order */
    qsort(table, 512, sizeof(seq_t), seq_cmp);
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready)
        build_table();

    if (n == 0)                /* offset is 1; behaviour for n==0 is undefined */
        return 0;

    if (n - 1 >= (idx_t)512)   /* more than 512 terms do not exist */
        return 0;

    return table[n - 1];
}

/*
 * A257693  Numbers such that the smallest nonzero digit present (A257679) in
 *          their factorial base representation is 3.
 * https://oeis.org/A257693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_smallest_nonzero_digit_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose factorial‑base representation has the smallest
   non‑zero digit equal to 3.  The sequence is indexed from 1 as in the
   OEIS entry.  The implementation lazily generates terms using a
   simple check on the factorial base expansion. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_smallest_nonzero_digit_three.h"

/* cache of generated terms */
static seq_t *buf = NULL;
static size_t len = 0;          /* number of terms cached */
static size_t cap = 0;          /* allocated capacity */
static seq_t next = 1;          /* next integer to test */

/* ensure the buffer can hold at least 'need' elements */
static void ensure_cap(size_t need)
{
    if (need <= cap)
        return;
    size_t newcap = cap ? cap * 2 : 16;
    while (newcap < need)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
    if (tmp == NULL)            /* OOM not expected in test harness */
        abort();
    buf = tmp;
    cap = newcap;
}

/* extend the cache until at least 'target' terms are stored */
static void fill_until(size_t target)
{
    while (len < target) {
        if (has_smallest_nonzero_digit_three(next)) {
            ensure_cap(len + 1);
            buf[len++] = next;
        }
        ++next;
    }
}

/* function required by the problem */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for the offset 1, but harmless */

    fill_until((size_t)n);
    return buf[n - 1];
}

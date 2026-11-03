/*
 * A334631  a(n) = number of letters in the sum of all previous terms, with
 *          a(1) = 1 (in US English, excluding spaces and hyphens).
 * https://oeis.org/A334631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1,
   a(n) = number of letters (US English, no spaces or hyphens)
          in the sum of all previous terms:
          a(n) = letter_count(∑_{i=1}^{n-1} a(i)) */
#include <stdlib.h>
#include "../utils/mem.h"

/* Count letters in the English word of an integer x
   (US English, no spaces or hyphens). */
static int letter_count(seq_t x)
{
    static const int ones_len[10]  = {0,3,3,5,4,4,3,5,5,4};
    static const int teens_len[10] = {3,6,6,8,8,7,7,9,8,8};
    static const int tens_len[10]  = {0,0,6,6,5,5,5,7,6,6};

    int total = 0;
    if (x >= 1000000000ULL) {            /* billions */
        seq_t q = x / 1000000000ULL;
        x   %= 1000000000ULL;
        total += letter_count(q) + 7;    /* "billion" */
    }
    if (x >= 1000000ULL) {               /* millions */
        seq_t q = x / 1000000ULL;
        x   %= 1000000ULL;
        total += letter_count(q) + 7;    /* "million" */
    }
    if (x >= 1000ULL) {                  /* thousands */
        seq_t q = x / 1000ULL;
        x   %= 1000ULL;
        total += letter_count(q) + 8;    /* "thousand" */
    }
    if (x >= 100ULL) {                   /* hundreds */
        seq_t q = x / 100ULL;
        x   %= 100ULL;
        total += letter_count(q) + 7;    /* "hundred" */
    }
    if (x >= 20ULL) {                    /* tens */
        total += tens_len[(int)(x / 10ULL)];
        x %= 10ULL;
        if (x > 0)
            total += ones_len[(int)x];
    } else if (x >= 10ULL) {             /* teens */
        total += teens_len[(int)(x - 10ULL)];
    } else if (x >= 1ULL) {              /* ones */
        total += ones_len[(int)x];
    }
    /* x == 0 contributes 0 letters */
    return total;
}

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;    /* a(i) values (1‑based) */
    static seq_t *cum = NULL;    /* cumulative sums ∑_{i=1}^{k} a(i) */
    static size_t cap = 0;       /* allocated capacity (array size) */
    static size_t len = 0;       /* largest n for which values are known */

    if (n == 0)
        return 0;                /* undefined for 0, return 0 for safety */

    /* Ensure we have space for index n (1‑based). Allocate one extra slot. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        new_cap++;                      /* one more slot for 1‑based indexing */

        seq_t *new_arr = mem_resize(seq_t, arr, new_cap);
        seq_t *new_cum = mem_resize(seq_t, cum, new_cap);
        if (!new_arr || !new_cum) {
            /* Allocation failed – terminate the program. */
            exit(1);
        }
        arr = new_arr;
        cum = new_cum;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            arr[1] = (seq_t)1;
            cum[1] = arr[1];
        } else {
            seq_t sumPrev = cum[i - 1];
            seq_t ai      = (seq_t)letter_count(sumPrev);
            arr[i]        = ai;
            cum[i]        = cum[i - 1] + ai;
        }
    }
    len = (size_t)n;
    return arr[n];
}

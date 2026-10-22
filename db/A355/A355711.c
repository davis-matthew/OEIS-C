/*
 * A355711  Starts of runs of 3 consecutive numbers with the same number of
 *          5-smooth divisors.
 * https://oeis.org/A355711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1247, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count5smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements OEIS A355711: the starting indices of runs of three
   consecutive numbers whose numbers of 5‑smooth divisors are equal.
   It performs a lazy search, storing results in a static array. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/count5smooth.h"

/* --------------------------------------------------------------------- */
/* Storage for the discovered run start indices. */
static seq_t *starts     = NULL;   /* array of run starts (seq_t values) */
static size_t starts_cap = 0;      /* current allocated capacity (elements) */
static size_t starts_len = 0;      /* number of run starts stored */

/* --------------------------------------------------------------------- */
/* Ensure the array holds at least `need` more elements.  Grows by
   doubling strategy. */
static void grow_starts(size_t need)
{
    if (starts_len + need <= starts_cap)
        return;

    size_t new_cap = starts_cap == 0 ? 8 : starts_cap << 1;
    while (starts_len + need > new_cap)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (starts) {
        for (size_t i = 0; i < starts_len; ++i)
            new_arr[i] = starts[i];
        free(starts);
    }
    starts     = new_arr;
    starts_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Next number to examine for the beginning of a run. */
static idx_t current_scan = 1;

/* --------------------------------------------------------------------- */
/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for index 0; return 0 */
        return (seq_t)0;

    /* Generate run starts until we have at least n of them. */
    while (starts_len < (size_t)n) {
        idx_t k = current_scan;

        seq_t c0 = count5smooth(k);
        seq_t c1 = count5smooth(k + 1);
        seq_t c2 = count5smooth(k + 2);

        if (c0 == c1 && c1 == c2) {    /* found a run of length 3 */
            grow_starts(1);
            starts[starts_len++] = (seq_t)k;
        }

        ++current_scan;
    }

    return starts[(size_t)n - 1];
}

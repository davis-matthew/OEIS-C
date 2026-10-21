/*
 * A290365  Numbers that cannot be written as a difference of 3-smooth numbers
 *          (A003586).
 * https://oeis.org/A290365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer that is NOT a difference of two
 *            3‑smooth numbers (numbers of the form 2^i·3^j, i,j ≥ 0)
 * Range:     u8 none, u16 n <= 31, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Parameters.  The bound must exceed a(10000).  For 64‑bit and bigger  */
/* builds 2 000 000 is ample; for 8‑bit builds the same value is kept  */
/* as it is still small enough (≈2 MB) over the static `mark` array. */
/* --------------------------------------------------------------------- */
#define MAX_BOUND 2000000

/* --------------------------------------------------------------------- */
/* Static storage.  The array of 3‑smooth numbers is small enough that a */
/* fixed‑size buffer works for all supported seq_t sizes.  The            */
/* `smooth_cnt` counter is updated on first use.  `mark[d] == 1` means   */
/* that `d` can be written as the difference of two 3‑smooth numbers.    */
/* --------------------------------------------------------------------- */
static seq_t  smooth[512];              /* 3‑smooth numbers ≤ MAX_BOUND  */
static size_t smooth_cnt = 0;           /* number of elements in smooth  */
static unsigned char mark[MAX_BOUND + 1] = {0};
static bool initialized = false;

/* --------------------------------------------------------------------- */
/* initialise()                                                               */
/* --------------------------------------------------------------------- */
static void initialise(void)
{
    if (initialized) return;

    /* Generate all 3‑smooth numbers ≤ MAX_BOUND.  Use idx_t for the      */
    /* intermediate multiplication so the loop works for all seq_t sizes. */
    for (idx_t a = (idx_t)1; a <= (idx_t)MAX_BOUND; a *= (idx_t)2) {
        for (idx_t b = a; b <= (idx_t)MAX_BOUND; b *= (idx_t)3) {
            if (smooth_cnt < sizeof(smooth)/sizeof(smooth[0]))
                smooth[smooth_cnt++] = (seq_t)b;
        }
    }

    /* Mark every positive difference of two distinct 3‑smooth numbers.  */
    for (size_t i = 0; i < smooth_cnt; ++i) {
        for (size_t j = 0; j < smooth_cnt; ++j) {
            if (i == j) continue;
            idx_t diff = (smooth[i] > smooth[j]) ?
                          (idx_t)smooth[i] - (idx_t)smooth[j] :
                          (idx_t)smooth[j] - (idx_t)smooth[i];
            if (diff >= 1 && diff <= (idx_t)MAX_BOUND)
                mark[(size_t)diff] = 1;
        }
    }

    initialized = true;
}

/* --------------------------------------------------------------------- */
/* Public function ------------------------------------------------------- */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    initialise();

    idx_t count = 0;
    for (size_t v = 1; v <= MAX_BOUND; ++v) {
        if (!mark[v]) {
            ++count;
            if (count == n)
                return (seq_t)v;
        }
    }
    /* If the bound is too small, return 0 as a sentinel value.   */
    return (seq_t)0;
}

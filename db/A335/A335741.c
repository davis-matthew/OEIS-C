/*
 * A335741  Number of Pell numbers (A000129) <= n.
 * https://oeis.org/A335741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A335741: number of Pell numbers (A000129) ≤ n.
   Computed by generating Pell numbers P(k) = 2*P(k-1)+P(k-2) until the
   next exceeds n, and counting how many have been generated.  The count
   is returned as seq_t. */

#include <stdlib.h>
#include <string.h>

/* Static cache for Pell numbers, stored as idx_t (uint64_t). */
static idx_t *pell = NULL;   /* array of Pell numbers */
static size_t cap  = 0;      /* capacity of pell array */
static size_t len  = 0;      /* number of Pell numbers stored */

/* Return the number of Pell numbers ≤ n. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (pell == NULL) {
        cap = 4;
        pell = (idx_t *)malloc(cap * sizeof(idx_t));
        pell[0] = (idx_t)0;   /* P(0) */
        pell[1] = (idx_t)1;   /* P(1) */
        len   = 2;
    }

    /* Grow cache until the next Pell number would exceed n. */
    while (1) {
        /* Compute next Pell number: P(k) = 2*P(k-1)+P(k-2). */
        if (len < 2) break;   /* safety check */

        idx_t next = 2 * pell[len - 1] + pell[len - 2];
        if (next > n) break;  /* all subsequent are larger */

        /* Ensure capacity for the new element. */
        if (len == cap) {
            size_t new_cap = cap * 2;
            idx_t *tmp = (idx_t *)malloc(new_cap * sizeof(idx_t));
            memcpy(tmp, pell, cap * sizeof(idx_t));
            free(pell);
            pell = tmp;
            cap  = new_cap;
        }
        pell[len++] = next;
    }

    /* Binary search to count how many Pell numbers ≤ n. */
    size_t lo = 0, hi = (len ? len - 1 : 0);
    size_t count = 0;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (pell[mid] <= n) {
            count = mid + 1;  /* indices 0 .. mid are ≤ n */
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    return (seq_t)count;
}

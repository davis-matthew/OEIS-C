/*
 * A328195  Maximum length of a divisibility chain of consecutive divisors of
 *          n greater than 1.
 * https://oeis.org/A328195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8798, u16 n <= 8798, u32 n <= 8798, u64 n <= 8798,
 *            u128 n <= 8798, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum length of a chain of consecutive divisors
   (each dividing the next) of n that are greater than 1.  The
   implementation enumerates all such divisors, sorts them, and
   scans for the longest consecutive‑divisibility block.  It runs in
   O(n log n) time (the sorting dominates) and uses O(d) memory
   where d is the number of divisors >1 of n.  This is fast enough
   for n up to about 10000. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------
   Helper: collect all divisors d of n with d > 1.
   The result array is malloc'ed; caller must free it.
   ------------------------------------------------------------------ */
static void get_divisors(uint64_t n, uint64_t **out, size_t *len)
{
    if (n <= 1) {
        *out = NULL;
        *len = 0;
        return;
    }

    uint64_t *arr = NULL;
    size_t cap = 0, cnt = 0;

    for (uint64_t d = 2; d <= n; ++d) {
        if (n % d == 0) {
            if (cnt == cap) {
                cap = (cap == 0) ? 4 : cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
                if (!tmp) abort(); /* allocation failure */
                arr = tmp;
            }
            arr[cnt++] = d;
        }
    }

    *out = arr;
    *len = cnt;
}

/* ------------------------------------------------------------------
   Comparator for qsort on uint64_t values.
   ------------------------------------------------------------------ */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

/* ------------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1)
        return 0;

    /* Enumerate divisors d > 1 */
    uint64_t *divs = NULL;
    size_t dlen = 0;
    get_divisors((uint64_t)n, &divs, &dlen);

    if (dlen == 0) {          /* should not happen for n > 1 */
        free(divs);
        return 0;
    }

    /* Sort the divisors */
    qsort(divs, dlen, sizeof(uint64_t), cmp_uint64);

    /* Scan for longest chain of consecutive divisibilities */
    size_t maxlen = 1, curlen = 1;
    for (size_t i = 0; i + 1 < dlen; ++i) {
        if (divs[i + 1] % divs[i] == 0)
            ++curlen;
        else {
            if (curlen > maxlen)
                maxlen = curlen;
            curlen = 1;
        }
    }
    if (curlen > maxlen)
        maxlen = curlen;

    free(divs);
    return (seq_t)maxlen;
}

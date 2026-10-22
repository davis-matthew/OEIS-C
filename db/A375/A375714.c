/*
 * A375714  Positions of non-successions of consecutive non-perfect-powers.
 *          Numbers k such that the k-th non-perfect-power is at least two
 *          fewer than the next.
 * https://oeis.org/A375714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = kth position k of a non‑succession of consecutive
 *            non‑perfect‑powers in A007916, i.e. A007916(k+1) – A007916(k) >
 *            1
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve of perfect powers up to the
   integer currently examined.  For each non‑perfect‑power encountered
   it increments the counter of non‑perfect‑powers seen; if the
   succeeding integer is a perfect power the current counter is
   recorded as a(n).  No pre‑allocation of the sequence itself is
   required, so the code works for both integer and arbitrary‑precision
   seq_t types. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Static data used to generate perfect powers up to a limit   */
/* ------------------------------------------------------------ */
static bool *pp = NULL;          /* flag array: true if index is a perfect power */
static idx_t pp_last = 0;        /* highest index that has been marked */

/* Expand the perfect‑power table to cover indices up to limit.  */
static void update_pp(idx_t limit)
{
    if (limit <= pp_last) return;

    /* Allocate a new larger table and initialise it to false.  */
    bool *new_arr = (bool *)calloc(limit + 1, sizeof(bool));
    if (!new_arr) exit(EXIT_FAILURE);

    /* Copy existing flags into the new array.  */
    if (pp) {
        for (idx_t i = 0; i <= pp_last; ++i) new_arr[i] = pp[i];
        free(pp);
    }

    /* Mark all perfect powers ≤ limit. */
    for (uint32_t base = 2; (uint64_t)base * base <= limit; ++base) {
        uint64_t val = (uint64_t)base * base;
        while (val <= limit) {
            new_arr[val] = true;
            val *= base;
        }
    }

    pp      = new_arr;
    pp_last = limit;
}

/* ------------------------------------------------------------ */
/*                 Main sequence function                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1 – return 0 for safety */

    idx_t cur = 2;          /* integer currently examined       */
    idx_t pos = 0;          /* number of non‑perfect‑powers seen */
    idx_t count = 0;        /* number of terms already recorded */

    while (1) {
        /* Make sure we know whether cur and cur+1 are perfect powers. */
        if (cur + 1 > pp_last) update_pp(cur + 1);

        if (!pp[cur]) {          /* cur is a non‑perfect‑power */
            pos++;               /* advance position counter   */
            if (pp[cur + 1]) {    /* next integer is a perfect power */
                ++count;
                if (count == n)   /* the nth recorded term found */
                    return (seq_t)pos;
            }
        }
        ++cur;                    /* move to the next integer   */
    }
}

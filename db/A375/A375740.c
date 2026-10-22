/*
 * A375740  Numbers k such that A007916(k+1) - A007916(k) = 1. In other words,
 *          the k-th non-perfect-power is 1 less than the next.
 * https://oeis.org/A375740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A375740: positions k of non‑perfect‑powers for which
   the next non‑perfect‑power differs by one.  The algorithm
   iterates the integers, keeps a table of perfect‑power flags,
   counts the non‑perfect‑powers, and returns the index of the
   k‑th pair of consecutive non‑perfect‑powers. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Dynamic table of perfect‑power flags (true if index is a perfect power). */
static bool *pp_table = NULL;   /* table pointer */
static idx_t pp_len   = 0;      /* current table size */

/* Ensure pp_table is large enough for indices up to 'limit'.
   The table is doubled until it exceeds that limit. */
static void grow_pp(idx_t limit)
{
    if (limit < pp_len)          /* already sized adequately */
        return;

    idx_t new_len = pp_len ? pp_len : 64;
    while (new_len <= limit)
        new_len <<= 1;           /* double size */

    bool *new_tbl = (bool*)calloc(new_len, sizeof(bool));
    if (!new_tbl)
        return;                 /* allocation failure – unlikely for used limits */

    /* Mark perfect powers a^e with a>=2, e>=2 and a^e < new_len. */
    for (idx_t base = 2; base * base < new_len; ++base) {
        unsigned __int128 val = (unsigned __int128)base * base;   /* start with e=2 */
        while (val < new_len) {
            new_tbl[(idx_t)val] = true;
            val *= base;       /* next power of base */
        }
    }

    if (pp_table)
        free(pp_table);         /* replace old table */
    pp_table = new_tbl;
    pp_len   = new_len;
}

/* Return the n-th term of sequence A375740.  n ≥ 1. */
seq_t a(idx_t n)
{
    idx_t found = 0;   /* number of qualifying positions found */
    idx_t pos   = 0;   /* current index among non‑perfect‑powers */
    idx_t num   = 1;   /* integer currently being examined, start at 1 */

    while (true) {
        ++num;                     /* consider next integer (2,3,…) */
        grow_pp(num + 1);           /* ensure table covers num and num+1 */

        if (!pp_table[num]) {       /* num is not a perfect power */
            ++pos;                  /* it is the pos‑th non‑perfect‑power */

            if (!pp_table[num + 1]) /* num+1 is also not a perfect power */
            {
                ++found;
                if (found == n)     /* found the n‑th qualifying position */
                    return (seq_t)pos;
            }
        }
    }
}

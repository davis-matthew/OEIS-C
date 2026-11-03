/*
 * A297145  Numbers whose base-9 digits d(m), d(m-1), ..., d(0) have m=0 or
 *          else d(i) = d(i+1) for some i in {0,1,...,m-1}.
 * https://oeis.org/A297145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A297145: Numbers whose base‑9 digits contain at least one
   pair of consecutive equal digits (single‑digit numbers are always
   accepted). The function builds the sequence incrementally and caches
   the generated terms so that repeated calls are efficient.
*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a number satisfies the property.  */
static bool has_consecutive_eq(seq_t x)
{
    if (x < 9) {                         /* single‑digit base‑9 numbers */
        return true;
    }
    seq_t prev = x % 9;
    x /= 9;
    while (x > 0) {
        seq_t d = x % 9;
        if (d == prev) return true;
        prev = d;
        x /= 9;
    }
    return false;
}

/* Return the nth term of the sequence (n ≥ 1).  */
seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of allocated elements   */
    static idx_t upto = 0;          /* number of terms stored         */
    static seq_t next_candidate = 1;/* next integer to test           */

    if (n == 0) return 0;          /* safety, though undefined   */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cap = 128;
        cache = mem_new(seq_t, cap);
    }

    /* generate terms until we have at least n entries */
    while (upto < n) {
        if (has_consecutive_eq(next_candidate)) {
            if ((size_t)upto >= cap) {
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[upto] = next_candidate;
            upto++;
        }
        next_candidate++;
    }
    return cache[n - 1];
}

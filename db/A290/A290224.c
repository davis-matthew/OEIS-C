/*
 * A290224  Numbers n such that A290223(n) = 0.
 * https://oeis.org/A290224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A290224
 *  Numbers n such that the iterative transformation
 *  n → | n - (sum of decimal digits of n)^2 | eventually reaches 0. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"   /* digit_sum(seq_t) */

/* Apply one step of the transformation. */
static seq_t step(seq_t x)
{
    seq_t s = digit_sum(x);
    seq_t sq = s * s;
    return (x >= sq) ? (x - sq) : (sq - x);
}

/* Detect whether the sequence starting at x reaches 0. */
static bool reaches_zero(seq_t x)
{
    seq_t slow = x;
    seq_t fast = x;
    for (;;)
    {
        slow = step(slow);
        if (slow == 0)
            return true;
        fast = step(step(fast));
        if (fast == 0)
            return true;
        if (slow == fast)
            return false;          /* cycle that does not contain 0 */
    }
}

/* Memoised list of found terms. */
static seq_t *found = NULL;       /* array of terms          */
static size_t fsize = 0;          /* number of terms stored  */
static size_t fcap = 0;           /* capacity of array       */
static idx_t next_scan = 1;       /* next integer to test    */

/* Ensure that at least n terms are stored in found. */
static void ensure_terms(idx_t n)
{
    while (fsize < n)
    {
        if (reaches_zero((seq_t)next_scan))
        {
            if (fsize == fcap)
            {
                size_t newcap = fcap == 0 ? 1 : fcap << 1;
                seq_t *tmp = (seq_t *)realloc(found, newcap * sizeof(seq_t));
                if (!tmp)
                    return;                /* allocation failed – abort */
                found = tmp;
                fcap = newcap;
            }
            found[fsize++] = (seq_t)next_scan;
        }
        next_scan++;
    }
}

/* External linkage for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset 0 */
    ensure_terms(n);
    return found[n - 1];
}

/*
 * A390326  Oblong numbers not of the forms 2^k*(2^k - 1) or 2^k*(2^k + 1).
 * https://oeis.org/A390326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390326: oblong numbers not of the
   forms 2^k*(2^k-1) or 2^k*(2^k+1).  The function
   generates the n-th such number by checking each
   oblong m*(m+1) and excluding those that match
   the forbidden patterns. */

/* We use the provided integer types 'seq_t' and 'idx_t'. */
#include <stdbool.h>

/* Helper: return true if oblong value ob equals
   2^k*(2^k-1) or 2^k*(2^k+1) for some k≥1. */
static bool is_excluded(seq_t ob)
{
    seq_t p2 = 2;   /* 2^1 */
    seq_t p4 = 4;   /* 4^1 = 2^2 */

    for (int k = 1; ; ++k) {
        /* Check both possible values for this k */
        if ((p4 - p2) == ob || (p4 + p2) == ob)
            return true;

        /* If both values are already larger than ob,
           further k will only increase them, so we can stop. */
        if ((p4 - p2) > ob && (p4 + p2) > ob)
            break;

        /* Guard against overflow when squaring; if
           the product would overflow, we can stop because
           values will exceed ob.  For unsigned types
           overflow yields a smaller number, so we break. */
        /* Using a simple heuristic: stop if next multiplication
           would exceed ob. */
        if (p4 > ob && p2 > ob)
            break;

        /* Advance to next k: 2^(k+1) and 4^(k+1) */
        p2 = p2 * 2;
        p4 = p4 * 4;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* Undefined for offset 1, but return 0 for safety */

    idx_t   count = 0;
    seq_t   m     = 1;

    for (;;) {
        seq_t ob = m * (m + 1);          /* m is oblong index */
        if (!is_excluded(ob)) {
            ++count;
            if (count == n)
                return ob;
        }
        ++m;
    }
}

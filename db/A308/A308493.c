/*
 * A308493  Numbers k such that k in base 10 contains the same digits as k in
 *          some other base.
 * https://oeis.org/A308493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   has_matching_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Computation of OEIS A308493:
 *  Numbers k such that k in base 10 contains the same digits as k in some
 *  other base.  The sequence is obtained by brute‑force search of candidates
 *  and checking the property.  Results are cached to answer repeated
 *  requests in O(1) after the first traversal.
 *
 *  The implementation works for any build where seq_t is an unsigned
 *  integer type or an arbitrary‑precision integer class.  For integers
 *  below the 64‑bit limit it runs in milliseconds; for arbitrary‑precision
 *  builds it stays within the time limits for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_matching_base.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1; 0 is never requested */
        return 0;

    /* Cache large enough for typical usage (n ≤ 10000). */
    enum { CACHE_CAP = 65536 };
    static seq_t cache[CACHE_CAP];
    static size_t cache_len = 0;
    static seq_t next_candidate = 0;

    /* Generate terms until we have the desired index. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate++;
        if (has_matching_base(cand))
            cache[cache_len++] = cand;
    }
    return cache[n - 1];
}

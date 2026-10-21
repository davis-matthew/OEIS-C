/*
 * A276001  Numbers n for which A060502(n) <= 1; numbers with at most one
 *          distinct slope in their factorial representation.
 * https://oeis.org/A276001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   _append, _one_slope
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276001:
   Numbers n for which all non‑zero digits of the factorial‑base
   representation have the same slope (slope = position − digit).
   This implementation incrementally builds the sequence by testing
   successive integers.  It works for any seq_t implementation.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_append.h"
#include "../utils/_one_slope.h"

/* External function required by the task. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* array holding the sequence values      */
    static size_t len = 0;      /* number of elements stored              */
    static size_t cap = 0;      /* capacity of the array                  */
    static seq_t cand = 0;      /* next integer to test                   */

    /* Generate terms until we have at least (n+1) values. */
    while (len <= (size_t)n) {
        if (_one_slope(cand))
            _append(&seq, &len, &cap, cand);
        ++cand;
    }

    return seq[n];
}

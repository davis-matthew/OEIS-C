/*
 * A331822  Starts of runs of 3 consecutive positive negabinary-Niven numbers
 *          (A331728).
 * https://oeis.org/A331822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 53, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9940 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A331822:  Starts of runs of 3 consecutive positive negabinary-Niven numbers.
    The sequence is produced by iterating all positive integers, testing
    whether each is a negabinary-Niven number (i.e., divisible by the
    number of 1's in its base-(-2) representation), keeping a running
    count of consecutive Niven numbers, and recording the start of every
    block of length at least three.

    The implementation caches the result list so that any call order
    works and subsequent queries are O(1).  The computation uses only
    integer arithmetic and dynamic allocation, meeting the input
    constraints.  The algorithm is O(n + a(n)) where a(n) is the n-th
    term value, well below 2 seconds for n up to ~10000.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if n is a negabinary-Niven number.  */
static bool negabinary_niven(seq_t n)
{
    /* Count ones in base -2 representation. */
    int popcnt = 0;
    int64_t val = (int64_t)n;          /* n is positive.       */
    while (val != 0) {
        int r = val & 1;               /* remainder 0 or 1.   */
        if (r) popcnt++;               /* count a 1.          */
        val = -(val - r) / 2;          /* next higher digit.  */
    }
    /* popcnt is >=1 for n>0.  Check divisibility. */
    return (n % popcnt) == 0;
}

/* Grow buf to at least new_cap elements.  */
static void ensure_capacity(seq_t **buf, size_t *cap, size_t new_cap)
{
    if (*cap < new_cap) {
        size_t new_size = (*cap == 0) ? 2 : (*cap << 1);
        if (new_size < new_cap) new_size = new_cap;
        seq_t *tmp = mem_resize(seq_t, *buf, new_size);
        *buf = tmp;
        *cap = new_size;
    }
}

/*  Returns the n-th term of the sequence.  */
seq_t a(idx_t n)
{
    static seq_t *buf = NULL;      /* cached start numbers. */
    static size_t cap = 0;         /* allocated capacity.   */
    static size_t sz = 0;          /* number of cached terms. */
    static seq_t cur = 1;          /* next integer to test. */
    static int consec = 0;         /* length of current run. */

    if (n == 0) return 0;          /* Not defined for offset 0. */

    while (sz < (size_t)n) {
        bool isNiven = negabinary_niven(cur);
        if (isNiven) consec++; else consec = 0;

        if (consec >= 3) {
            ensure_capacity(&buf, &cap, sz + 1);
            buf[sz++] = cur - 2;   /* start of the 3-consecutive block. */
        }
        cur++;
    }
    return buf[n - 1];
}

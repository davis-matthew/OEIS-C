/*
 * A268996  Number of 2 X n binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268996
   Empirical recurrence:
   a(n) = 2*a(n-1) + 5*a(n-2) - 4*a(n-3) - 11*a(n-4) - 6*a(n-5) - a(n-6)
   Initial values:
   a(1) = 4, a(2) = 13, a(3) = 35, a(4) = 103, a(5) = 278, a(6) = 763
*/

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n ≤ 0, return 0 safely */

    /* Allocate space for the terms up to n */
    size_t sz = (size_t)n;        /* n is unsigned, cast to size_t for indexing */
    if (sz == 0)
        sz = 1;                   /* ensure non‑zero allocation when n==0 */
    seq_t *a_arr = alloc_seq(sz);

    /* Set the first six values */
    if (n >= 1) a_arr[0] = (seq_t)4;
    if (n >= 2) a_arr[1] = (seq_t)13;
    if (n >= 3) a_arr[2] = (seq_t)35;
    if (n >= 4) a_arr[3] = (seq_t)103;
    if (n >= 5) a_arr[4] = (seq_t)278;
    if (n >= 6) a_arr[5] = (seq_t)763;

    /* Compute further terms using the recurrence */
    for (size_t i = 7; i <= (size_t)n; ++i) {
        seq_t val = 0;
        val += (seq_t)2 * a_arr[i - 2];
        val += (seq_t)5 * a_arr[i - 3];
        val -= (seq_t)4 * a_arr[i - 4];
        val -= (seq_t)11 * a_arr[i - 5];
        val -= (seq_t)6 * a_arr[i - 6];
        val -= a_arr[i - 7];
        a_arr[i - 1] = val;
    }

    seq_t result = a_arr[n - 1];
    free_seq(a_arr);
    return result;
}

/*
 * A355568  Numbers k > 4 in a Collatz trajectory reaching k after starting at
 *          k-1.
 * https://oeis.org/A355568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the n-th integer k > 4 such that the Collatz trajectory
 *            starting at k–1 reaches k. Implementation: iteratively search
 *            for the next k, testing each predecessor's trajectory until l
 *            the required index is found
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Forward declaration of the helper function that follows.
   The function works with the integer type seq_t so that it can
   operate on arbitrary‑precision integers as well as built‑in
   unsigned integers. */
static bool collatz_hits(seq_t start, seq_t target);

/* External linkage function required by the problem. */
seq_t a(idx_t n)
{
    idx_t                count = 0;   /* how many valid k we have found   */
    seq_t                k     = 5;     /* start checking at the first k > 4 */

    for (;;)
    {
        /* Test whether the Collatz trajectory of k–1 reaches k */
        if (collatz_hits(k - 1, k))
        {
            ++count;
            if (count == n)
                return k;            /* found the n‑th required value */
        }

        ++k;                           /* examine the next candidate */
    }
}

/* Test whether the Collatz trajectory of `start` eventually hits
   `target`.  The traversal stops when it reaches `target` or
   the number 1; in the latter case the target was not reached. */
static bool collatz_hits(seq_t start, seq_t target)
{
    seq_t x = start;

    while (x != target && x != 1)
    {
        /* Use the modulo 2 test to determine if x is even or odd
           so that the function works for arbitrary‑precision ints
           that support the % operator. */
        if ((x % 2) == 0)
            x /= 2;          /* even case: divide by 2 */
        else
            x = x * 3 + 1;   /* odd case: 3x+1 */
    }
    return x == target;
}

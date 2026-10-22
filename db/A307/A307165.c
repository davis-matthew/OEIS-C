/*
 * A307165  Numbers k such that the sequence f(0)=f(1)=k, f(x)=(a*b) mod
 *          (a+b+1), where a=f(x-1) and b=f(x-2) is a cycle.
 * https://oeis.org/A307165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_cycle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) : the n-th integer k such that the Abmod sequence
 *        starting with (k,k) is periodic and eventually returns
 *        to the pair (k,k).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_cycle.h"

/* Static storage for memoised sequence values */
static seq_t *seq          = NULL;     /* array of found k values      */
static idx_t  terms        = 0;        /* number of values stored     */
static seq_t  next_k       = 0;        /* next candidate to test      */
static size_t cap          = 0;        /* allocated capacity of seq   */

/* Ensure the array has at least 'needed' slots */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;
    size_t newcap = (cap == 0) ? 16 : cap * 2;
    while (newcap < needed)
        newcap *= 2;

    seq_t *newp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    if (!newp)
        abort();                      /* memory exhausted */

    seq  = newp;
    cap  = newcap;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; caller may ignore n==0 */

    while (terms < n)
    {
        if (is_cycle(next_k))
        {
            ensure_capacity(terms + 1);
            seq[terms++] = next_k;
        }
        next_k++;
    }

    return seq[n - 1];
}

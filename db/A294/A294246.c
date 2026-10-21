/*
 * A294246  Sum of the smaller parts of the partitions of 2n into two parts
 *          with smaller part nonsquarefree.
 * https://oeis.org/A294246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} i * (1 - mu(i)^2) (sum of non‑squarefree
 *            numbers up to n)
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mobius.h"   /* assumes seq_t mobius(idx_t) is defined elsewhere */

static seq_t *prefix = NULL;   /* prefix[i] = a(i) for 0 <= i <= pref_max */
static idx_t  pref_max = 0;

/* Return the n‑th term of sequence A294246.  The function lazily expands
   a static cache so that repeated calls (in any order) are efficient. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* initialise storage if this is the first call */
    if (prefix == NULL) {
        prefix   = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (prefix == NULL)
            abort();                 /* out of memory */
        prefix[0] = (seq_t)0;
        pref_max = 0;
    }

    /* extend the array and recompute missing values if necessary */
    if (n > pref_max) {
        seq_t *new_prefix = (seq_t *)realloc(prefix, sizeof(seq_t) * (n + 1));
        if (new_prefix == NULL)
            abort();                 /* out of memory */
        prefix = new_prefix;

        for (idx_t i = pref_max + 1; i <= n; ++i) {
            seq_t previous = prefix[i - 1];
            seq_t mob = mobius(i);                     /* 0 if i is not squarefree */
            seq_t add = (mob == (seq_t)0) ? (seq_t)i : (seq_t)0;
            prefix[i] = previous + add;
        }
        pref_max = n;
    }

    return prefix[n];
}

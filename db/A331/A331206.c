/*
 * A331206  Numbers k such that A053985(k) divides k.
 * https://oeis.org/A331206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that A053985(k) divides k, where
 *            A053985(k) is the value of the binary representation of k
 *            interpreted in base -2
 * Range:     u8 n <= 52, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   eval_neg2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/eval_neg2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – guard */

    /* Static cache of found terms */
    static seq_t *terms = NULL;
    static size_t sz = 0, cap = 0;
    static uint64_t next_k = 1;           /* smallest k to test next */

    /* Generate terms until we have at least n of them */
    while ((idx_t)sz < n)
    {
        uint64_t k = next_k++;
        long long v = eval_neg2(k);
        if (v != 0 && ((long long)k % v) == 0)
        {
            if (sz == cap)                /* grow the array */
            {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                terms = tmp;
                cap = newcap;
            }
            terms[sz++] = (seq_t)k;       /* store the term */
        }
    }
    return terms[n - 1];
}

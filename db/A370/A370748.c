/*
 * A370748  a(0)=1; a(n) = sum of all previous terms, eliminating repeated
 *          digits (starting from the left).
 * https://oeis.org/A370748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 1; a(n) = A137564( Σ_{k=0}^{n-1} a(k) ), where A137564
 *            preserves only the leftmost occurrence of each decimal digit in
 *            the given number
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 1086, u64 n <= 9570,
 *            u128 n <= 9570, big any n
 * Helpers:   removeDupDigits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/removeDupDigits.h"

seq_t a(idx_t n)
{
    static seq_t *A    = NULL;   /* sequence terms          */
    static seq_t *Cum  = NULL;   /* cumulative sums        */
    static size_t sz   = 0;      /* number of stored terms  */
    static size_t cap  = 0;      /* allocated capacity      */

    size_t idx = (size_t)n;

    /* Extend cache if needed. */
    if (idx >= sz) {
        size_t need = idx + 1;
        if (cap < need) {
            size_t newCap = cap ? cap * 2 : 1;
            while (newCap < need) newCap *= 2;
#ifdef __cplusplus
            seq_t *newA   = new seq_t[newCap];
            seq_t *newCum = new seq_t[newCap];
            for (size_t i = 0; i < sz; ++i) {
                newA[i]   = A[i];
                newCum[i] = Cum[i];
            }
            delete[] A;
            delete[] Cum;
            A   = newA;
            Cum = newCum;
#else
            A   = (seq_t*)realloc(A,   newCap * sizeof(seq_t));
            Cum = (seq_t*)realloc(Cum, newCap * sizeof(seq_t));
#endif
            cap = newCap;
        }

        if (sz == 0) {
            A[0]   = (seq_t)1;
            Cum[0] = (seq_t)1;
            sz = 1;
        }

        for (size_t i = sz; i < need; ++i) {
            seq_t next = removeDupDigits(Cum[i - 1]);
            A[i]   = next;
            Cum[i] = Cum[i - 1] + next;
        }
        sz = need;
    }

    return A[idx];
}

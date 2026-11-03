/*
 * A242286  Positive integers k for which the k-th triangular number is
 *          greedy-summable.
 * https://oeis.org/A242286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) gives the n‑th positive integer k such that the k‑th
 *            triangular number T(k)=k(k+1)/2 can be written as a sum of
 *            distinct earlier triangular numbers by the greedy algorithm
 * Range:     u8 n <= 15, u16 n <= 352, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 936 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Greedy‑summability test for a single k.
   Uses only the formula for triangular numbers to avoid
   any incorrect dynamic caching. */
static bool greedy_summable(idx_t k)
{
    seq_t rem = (seq_t)k * ((seq_t)k + 1) / 2;   /* T(k) */
    idx_t last = k - 1;                           /* cannot use T(k) itself */

    while (rem > 0) {
        bool chosen = false;
        for (idx_t i = last; i > 0; --i) {
            seq_t t = (seq_t)i * ((seq_t)i + 1) / 2;   /* T(i) */
            if (t <= rem) {
                rem -= t;
                last = i - 1;      /* next summands must be strictly smaller */
                chosen = true;
                break;
            }
        }
        if (!chosen)   /* cannot reduce remainder */
            return false;
    }
    return true;   /* completely represented */
}

/* Output sequence a(n) --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Cached list of indices k that are greedy‑summable. */
    static seq_t *Vals = NULL;
    static size_t ValsCap = 0;     /* allocated capacity of Vals */
    static size_t ValsLen = 0;     /* number of valid entries */
    static idx_t NextK = 1;        /* next k to test for greedy_summability */

    if (n == 0)    /* safety: offset starts at 1 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (ValsLen < n) {
        if (greedy_summable(NextK)) {
            if (ValsLen == ValsCap) {
                ValsCap = (ValsCap == 0) ? 4 : ValsCap * 2;
                Vals = mem_resize(seq_t, Vals, ValsCap);
            }
            Vals[ValsLen++] = (seq_t)NextK;
        }
        ++NextK;
    }
    return Vals[n - 1];
}

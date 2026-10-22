/*
 * A336033  a(n) is the number of k such that 1 <= k < n and a(k) XOR ... XOR
 *          a(n-1) = 0 (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A336033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k such that 1 ≤ k < n and a(k) XOR … XOR a(n-1)
 *            = 0, where XOR is the bitwise XOR operator
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */

/*  External integer types supplied by the build environment  */
seq_t a(idx_t n)
{
    /* static cache of already computed terms and prefixes          */
    static seq_t  *seq  = NULL;   /* a[1 .. max_computed]          */
    static seq_t  *pref = NULL;   /* pref[0 .. max_computed]       */
    static idx_t   max_computed = 0; /* largest n for which data exist */
    static size_t  alloc_size   = 0; /* allocated array capacity (>= max+1) */

    /* n < 1 is outside the OEIS offset – return 0 for safety */
    if (n < 1)
        return (seq_t)0;

    /* If the requested n is beyond what we have already computed,
       extend the cached arrays and compute all missing terms.          */
    if (n > max_computed) {
        /* ensure enough storage for indices 0 .. n (pref needs n+1 entries) */
        size_t needed = (size_t)(n + 1);
        if (needed > alloc_size) {
            size_t new_cap = needed < 16 ? 16 : needed * 2;
            seq_t *new_seq  = (seq_t *)realloc(seq,  new_cap * sizeof(seq_t));
            seq_t *new_pref = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
            if (!new_seq || !new_pref) {
                /* memory allocation failed – program terminates */
                abort();
            }
            seq = new_seq;
            pref = new_pref;
            alloc_size = new_cap;
        }

        /* initialise pref[0] on the first run */
        if (max_computed == 0) {
            pref[0] = 0;
        }

        /* Compute a(i) and pref(i) for i = max_computed+1 … n      */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t target = pref[i - 1];   /* pref(n-1) for this i       */
            idx_t cnt = 0;

            /* count how many j in [0, i-2] have pref[j] == target   */
            for (idx_t j = 0; j + 1 < i; ++j) {
                if (pref[j] == target)
                    ++cnt;
            }

            seq[i] = (seq_t)cnt;
            pref[i] = pref[i - 1] ^ seq[i]; /* update prefix XOR */
        }

        max_computed = n;
    }

    return seq[n];
}

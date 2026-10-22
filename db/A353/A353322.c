/*
 * A353322  A variant of Van Eck's sequence where we only consider powers of
 *          2: for n >= 0, if a(n) = a(n-2^e) for some e, take the least such
 *          e and set a(n+1) = 2^e; otherwise a(n+1) = 0. Start with a(1) = 0.
 * https://oeis.org/A353322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 For n >= 1: let e be the least integer >= 0 such that
 *            a(n) == a(n - 2^e) (with 2^e <= n-1). If such an e exists, then
 *            a(n+1) = 2^e; otherwise a(n+1) = 0. This matches the description
 *            in OEIS A353322
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* 1‑based array of computed terms; index 0 is unused. */
    static seq_t *seq = NULL;   /* sequence storage */
    static size_t cap = 0;      /* allocated size (including unused index 0) */
    static idx_t  len = 0;      /* last index that has a value */

    if (n < 1)
        return (seq_t)0; /* undefined for 0, return 0 for safety */

    /* Ensure capacity for indices up to n (plus one spare to track size). */
    if (cap < (size_t)(n + 2)) {
        size_t new_cap = n + 2;
        if (new_cap < 16)
            new_cap = 16;          /* small start size */

        seq_t *tmp = NULL;

#ifdef __cplusplus
        /* Allocate new array using C++ new (to construct objects). */
        tmp = new (std::nothrow) seq_t[new_cap];
        if (!tmp)
            abort();                  /* allocation failed */
        /* Copy existing values to the new array. */
        if (seq) {
            for (idx_t i = 1; i <= len; ++i)
                tmp[i] = seq[i];
            delete[] seq;              /* free old array */
        }
#else
        /* Allocate new array using C realloc (seq_t is a POD). */
        tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                  /* allocation failed */
#endif
        seq = tmp;
        cap = new_cap;
    }

    /* Initialise first element on first call. */
    if (len < 1) {
        seq[1] = (seq_t)0;  /* a(1) = 0 */
        len = 1;
    }

    /* Compute the sequence up to the requested index n. */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t prev = seq[i - 1];
        seq_t val  = (seq_t)0;

        /* Find the smallest exponent e such that
           a(i-1) == a(i-1-2^e), with 2^e <= i-2 */
        for (idx_t e = 0; e < 64; ++e) {
            idx_t offset = ((idx_t)1) << e;              /* offset = 2^e */
            if (offset > (idx_t)(i - 2))
                break;                                  /* no larger e possible */
            if (prev == seq[(idx_t)(i - 1 - offset)]) {
                val = (seq_t)offset;                     /* a(i) = 2^e */
                break;
            }
        }

        seq[i] = val;
    }

    len = n;
    return seq[n];
}

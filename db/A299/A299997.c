/*
 * A299997  Lexicographic first sequence of positive integers such that
 *          a(n)*a(n+1) has a digit 7, and no term occurs twice.
 * https://oeis.org/A299997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   has_digit7_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299997: lexicographic first permutation of positive integers
 * such that the product of consecutive terms contains a digit 7 and all terms
 * are distinct.
 *
 * Algorithm:
 *   a(1) = 1.
 *   For n > 1, a(n) is the smallest positive integer not yet used for which
 *   a(n‑1) * a(n) contains the digit 7.
 *   The function stores all terms it generates in a static array so that
 *   subsequent calls with smaller or larger n are answered instantly.
 *   A bitmap of used integers is maintained; the bitmap grows as needed.
 *   Only C standard types, <stdbool.h>, <stdint.h> and <stdlib.h> are used,
 *   so the code compiles as C11 and as C++17.  For builds where seq_t
 *   is an arbitrary‑precision class the necessary operators are used
 *   directly; for native integer seq_t the product is computed in
 *   unsigned __int128 to avoid overflow. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_digit7_seq_t.h"

/* Ensure that the used bitmap can address indices up to `upTo` (inclusive).
 * The bitmap is stored as a contiguous array of bool values indexed by
 * the integer value itself.  The array is resized by doubling its capacity. */
static void ensure_used_capacity(idx_t upTo, bool **used, idx_t *cap)
{
    if (upTo < *cap)
        return;
    idx_t newCap = (*cap == 0) ? 8 : *cap;
    while (newCap <= upTo)
        newCap <<= 1;
    bool *newArr = (bool *)realloc(*used, newCap * sizeof(bool));
    if (!newArr) abort();           /* allocation failed */
    for (idx_t i = *cap; i < newCap; ++i)
        newArr[i] = false;
    *used = newArr;
    *cap  = newCap;
}

/* Ensure that the sequence array can hold at least `n` terms (1‑based). */
static void ensure_seq_capacity(idx_t n, seq_t **seq, idx_t *cap)
{
    if (n <= *cap)
        return;
    idx_t newCap = (*cap == 0) ? 8 : *cap;
    while (newCap < n)
        newCap <<= 1;
    seq_t *newArr = (seq_t *)realloc(*seq, newCap * sizeof(seq_t));
    if (!newArr) abort();           /* allocation failed */
    *seq = newArr;
    *cap = newCap;
}

/* Public sequence function.  The sequence is memoised in static storage. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined, but return 0 safely */
        return 0;

    static seq_t *seq  = NULL;     /* array of terms, 0‑based (a(1) -> seq[0]) */
    static idx_t seq_len = 0;      /* number of terms stored so far */
    static idx_t seq_cap = 0;      /* capacity of seq array */

    static bool *used = NULL;      /* bitmap of used integers (1‑based) */
    static idx_t used_cap = 0;     /* capacity of used bitmap */

    /* Grow the memoised data structures if needed. */
    if (n > seq_len) {
        /* Ensure room for all terms up to n */
        ensure_seq_capacity(n, &seq, &seq_cap);

        /* Compute missing terms sequentially. */
        for (idx_t i = seq_len; i < n; ++i) {
            if (i == 0) {          /* a(1) */
                seq[0] = (seq_t)1;
                ensure_used_capacity(1, &used, &used_cap);
                used[1] = true;
                continue;
            }

            seq_t prev = seq[i - 1];
            seq_t candidate = (seq_t)1;

            for (;;) {
                /* Use candidate as an index in the bitmap.  The
                 * candidate value is assumed to fit in idx_t. */
                idx_t candIdx = (idx_t)candidate;
                ensure_used_capacity(candIdx, &used, &used_cap);
                if (used[candIdx]) {
                    /* Already used – try next integer. */
                } else if (has_digit7_seq_t(prev, candidate)) {
                    /* Found the required term. */
                    seq[i] = candidate;
                    used[candIdx] = true;
                    break;
                }
                ++candidate;          /* try next integer */
            }
        }
        seq_len = n;
    }

    return seq[n - 1];
}

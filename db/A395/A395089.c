/*
 * A395089  Binary complement of A395088.
 * https://oeis.org/A395089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9147, u16 n <= 9147, u32 n <= 9147, u64 n <= 9147,
 *            u128 n <= 9147, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Morphic binary sequence defined by the substitution:
 *
 *     1 → 100
 *     0 → 1
 *
 * This function returns the nth element a(n) (0‑based indexing) of
 * the resulting infinite word.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Linear recursion on the substitution tree.  */

    /* Enough room for the word lengths up to 2^64.  In each step the
     * length roughly doubles, so 70 levels are far more than enough.  */
    const size_t MAXK = 70;
    idx_t len0[MAXK], len1[MAXK];

    len0[0] = len1[0] = (idx_t)1;   /* length of a single symbol 0 or 1 */
    size_t k = 0;                   /* current depth being built   */

    /* Build the lengths of the word produced by expanding '1' k times.
     * Stop when len1[k] > n – we then have the minimal depth that
     * contains the nth position.  */
    while (len1[k] <= n) {
        if (k + 1 >= MAXK) break;           /* safety guard */

        len0[k + 1] = len1[k];                       /* 0 → 1           */

        /* Use 128‑bit arithmetic to avoid overflow when adding
         * len1[k] + 2 * len0[k].  If the result exceeds n we
         * simply store any value > n (the exact value is not
         * needed for the recursion).                     */
        __uint128_t sum = (__uint128_t)len1[k] + (__uint128_t)2 * len0[k];
        idx_t next = (sum > (unsigned long long)n) ? (idx_t)(n + 1)
                                                   : (idx_t)sum;
        len1[k + 1] = next;
        ++k;
    }

    /* Descend the substitution tree to locate the bit at position n. */
    idx_t pos   = n;
    int   symbol = 1;          /* start from the root symbol '1' */
    size_t depth = k;          /* depth of the expanded word  */

    while (1) {
        if (depth == 0)          /* reached a leaf (single symbol) */
            return (seq_t)symbol;

        if (symbol == 1) {
            idx_t l1 = len1[depth - 1];   /* length of an expanded '1' */
            idx_t l0 = len0[depth - 1];   /* length of an expanded '0' */

            if (pos < l1) {              /* first subword (expanded 1)   */
                /* stay with symbol 1 */
                depth--;
            } else if (pos < l1 + l0) {   /* first '0' block              */
                pos -= l1;
                symbol = 0;
                depth--;
            } else {                     /* second '0' block             */
                pos -= l1 + l0;
                symbol = 0;
                depth--;
            }
        } else {                         /* symbol == 0, maps to a single 1 */
            symbol = 1;                 /* go one level deeper          */
            depth--;
        }
    }
}

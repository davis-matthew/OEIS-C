/*
 * A276793  Indicator function for A003144.
 * https://oeis.org/A276793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - t(n) where t(n) is the nth bit of the binary
 *            Tribonacci word obtained by the morphism 0 → 01, 1 → 02, 2 → 0
 *            and mapping the word symbols to {0,1,1} (0→0, 1→1, 2→1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (not expected). */
    if (n == 0)
        return (seq_t)0;

    /* Lazy generation of the Tribonacci word up to the requested index. */
    static int  *trib   = NULL;    /* current word, symbols 0,1,2   */
    static size_t trib_len = 0;    /* length of the word            */

    size_t need = (size_t)n;       /* convert 1‑based n to size_t    */

    /* If the word is not yet initialized, start with "0". */
    if (trib_len == 0) {
        trib = (int *)malloc(sizeof(int));
        if (!trib)
            return (seq_t)0;       /* out of memory guard           */
        trib[0] = 0;
        trib_len = 1;
    }

    /* Expand the word until it contains at least 'need' symbols. */
    while (trib_len < need) {
        size_t old_len = trib_len;
        size_t new_cap = old_len * 2 + 10;     /* ample capacity for the new word   */
        int *new_trib = (int *)malloc(new_cap * sizeof(int));
        if (!new_trib)
            return (seq_t)0;                   /* out of memory guard                */
        size_t new_len = 0;

        for (size_t i = 0; i < old_len; ++i) {
            int c = trib[i];
            if (c == 0) {
                new_trib[new_len++] = 0;
                new_trib[new_len++] = 1;
            } else if (c == 1) {
                new_trib[new_len++] = 0;
                new_trib[new_len++] = 2;
            } else { /* c == 2 */
                new_trib[new_len++] = 0;
            }
        }

        free(trib);
        trib = new_trib;
        trib_len = new_len;
    }

    /* The binary Tribonacci bit is 0 for symbol 0 and 1 for symbols 1 or 2.
       a(n) = 1 if the bit is 0, otherwise 0. */
    int bit = trib[need - 1];
    return (seq_t)(bit == 0 ? 1 : 0);
}

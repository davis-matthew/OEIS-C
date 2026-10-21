/*
 * A284944  Fixed point of the morphism 0 -> 01, 1 -> 1110.
 * https://oeis.org/A284944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1791, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>1, a(n) is the nth symbol in the infinite word obtained
   as the fixed point of the morphism
      0 → 01
      1 → 1110
   The algorithm computes the depth d such that the word σ^d(0) contains at least n
   symbols, then walks back through the expansions to locate the n-th symbol.
   The computation uses only integer arithmetic on seq_t. */

#include <stddef.h>

/* Return the n-th term (1‑indexed). */
seq_t a(idx_t n)
{
    /* Guard against n==0; the function is only defined for n≥1. */
    if (n == 0)
        return (seq_t)0;

    /* Maximum depth needed for any 64‑bit n is well below 40; 128 is safe. */
    enum { MAX_DEPTH = 128 };
    seq_t len0[MAX_DEPTH];
    seq_t len1[MAX_DEPTH];

    /* Base level: σ⁰(0) = “0”, σ⁰(1) = “1”. */
    len0[0] = 1;
    len1[0] = 1;
    size_t depth = 0;

    /* Increase depth until the first word is long enough. */
    while (len0[depth] < (seq_t)n) {
        ++depth;
        len0[depth] = len0[depth - 1] + len1[depth - 1];
        len1[depth] = 3 * len1[depth - 1] + len0[depth - 1];
    }

    /* Zero‑based position inside σ^depth(0). */
    seq_t pos = (seq_t)n - 1;
    int letter = 0;   /* the current symbol before expanding */

    /* Walk back from depth to the base word. */
    while (depth > 0) {
        if (letter == 0) {
            /* 0 → 0 1 */
            seq_t l0 = len0[depth - 1];
            if (pos < l0) {
                letter = 0;        /* still in the first child */
            } else {
                pos -= l0;
                letter = 1;        /* second child */
            }
            --depth;
        } else {
            /* 1 → 1 1 1 0 */
            seq_t l1 = len1[depth - 1];
            if (pos <  l1) {
                letter = 1;
            } else if (pos < 2 * l1) {
                pos -= l1;
                letter = 1;
            } else if (pos < 3 * l1) {
                pos -= 2 * l1;
                letter = 1;
            } else {
                pos -= 3 * l1;
                letter = 0;
            }
            --depth;
        }
    }
    return (seq_t)letter;
}

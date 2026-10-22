/*
 * A305490  Fixed point of the morphism 0->120, 1->110, 2->100.
 * https://oeis.org/A305490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth symbol (0‑2) of the fixed point of the morphism
 *            0→120, 1→110, 2→100, starting from the initial symbol 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Mapping from parent symbol to child symbol for each of the three
       positions (0, 1, 2) in the expansion.  Index 0 corresponds to the
       leftmost character in the expansion.  The fixed point is generated
       by repeatedly substituting each symbol by the string above. */
    static const int child[3][3] = {
        {1, 2, 0},   /* 0 → 1 2 0 */
        {1, 1, 0},   /* 1 → 1 1 0 */
        {1, 0, 0}    /* 2 → 1 0 0 */
    };

    /* Convert the 1‑based index n to a 0‑based position. */
    idx_t pos = n - 1;

    /* Find the smallest power of 3 that is ≥ n – this is the length of
       the word at the deepest level that contains position n. */
    idx_t len = 1;
    while (len < n) {
        /* len will never overflow for the sizes we need (≤ 10⁴
           and still fast for many thousands of calls). */
        len *= 3;
    }

    /* Walk from the root symbol (1) down the expansion tree,
       following the digits of pos in base‑3 from most significant to
       least significant.  At each level we choose the child according
       to the current digit and update the current symbol. */
    int cur = 1;  /* start symbol of the fixed point */
    while (len > 1) {
        len /= 3;                     /* size of a child block */
        int digit = (int)(pos / len); /* 0, 1, or 2 */
        pos %= len;                   /* remainder for next level */
        cur = child[cur][digit];
    }

    return (seq_t)cur;
}

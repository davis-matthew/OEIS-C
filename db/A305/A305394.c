/*
 * A305394  First differences of A140103.
 * https://oeis.org/A305394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth symbol of the fixed point of the morphism 5 → 54, 4 →
 *            53, 3 → 5 (ternary tribonacci word on {5,4,3})
 * Range:     u8 n <= 8442, u16 n <= 8442, u32 n <= 8442, u64 n <= 8442,
 *            u128 n <= 8442, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* target is 1‑based position in the infinite word */
    uint64_t target = (uint64_t)n;

    /* lengths of the word obtained after d substitutions */
    uint64_t l5[200], l4[200], l3[200];
    size_t depth = 0;                 /* current depth counter */
    l5[0] = l4[0] = l3[0] = 1;        /* word of length 1 for each symbol */

    /* Build lengths until the length of the word starting with 5
       reaches or exceeds the required position. */
    while (l5[depth] < target && depth + 1 < sizeof(l5) / sizeof(l5[0])) {
        depth++;
        l5[depth] = l5[depth-1] + l4[depth-1];   /* 5 → 5 4 */
        l4[depth] = l5[depth-1] + l3[depth-1];   /* 4 → 5 3 */
        l3[depth] = l5[depth-1];                /* 3 → 5   */
    }

    /* Recursive descent to locate the nth symbol.
       We start with the symbol 5 and traverse the tree of substitutions
       depth levels deep. */
    char cur = 5;   /* symbol at the current node (5,4 or 3) */
    while (depth > 0) {
        if (cur == 5) {
            /* children: 5 then 4 */
            if (target <= l5[depth-1]) {
                cur = 5;
            } else {
                target -= l5[depth-1];
                cur = 4;
            }
        } else if (cur == 4) {
            /* children: 5 then 3 */
            if (target <= l5[depth-1]) {
                cur = 5;
            } else {
                target -= l5[depth-1];
                cur = 3;
            }
        } else {        /* cur == 3 */
            /* child: 5 only */
            cur = 5;
        }
        depth--;        /* go one level deeper */
    }

    return (seq_t)cur;
}

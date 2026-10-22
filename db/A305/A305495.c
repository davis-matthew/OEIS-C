/*
 * A305495  Positions of 0 in the fixed point of the morphism 0->120, 1->110,
 *          2->100 applied to 1 (as in A305490).
 * https://oeis.org/A305495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth position of 0 in the fixed point of the morphism
   0->120, 1->110, 2->100 applied to 1.
   The function computes the nth zero by iterating over positions
   and determining the symbol at each position via base‑3 digit
   traversal of the morphism. */

#include <stddef.h>
#include <stdbool.h>

static const unsigned char trans[3][3] = {
    {1, 2, 0}, /* 0 → 1 2 0 */
    {1, 1, 0}, /* 1 → 1 1 0 */
    {1, 0, 0}  /* 2 → 1 0 0 */
};

/* Return true if the symbol at position pos (1‑based) is 0. */
static inline bool is_zero(idx_t pos)
{
    idx_t x = pos - 1;            /* position in 0‑based form */
    unsigned char stack[64];       /* base‑3 digits, least‑significant first */
    int sz = 0;
    while (x) {
        stack[sz++] = x % 3;
        x /= 3;
    }
    unsigned char sym = 1;        /* start from the seed symbol '1' */
    while (sz--) {
        sym = trans[sym][stack[sz]];
    }
    return sym == 0;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t count = 0;
    for (idx_t i = 1; ; ++i) {
        if (is_zero(i)) {
            ++count;
            if (count == n) {
                result = (seq_t)i;
                break;
            }
        }
    }
    return result;
}

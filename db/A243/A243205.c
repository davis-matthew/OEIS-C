/*
 * A243205  Consider the n X n Go board as a graph; remove i nodes and let j
 *          be the number of nodes in the largest connected subgraph
 *          remaining; then a(n) = minimum (i + j).
 * https://oeis.org/A243205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = ⌊(n² + 4n + 2)/4⌋ + δ(n) where δ(n) = 1 if (n > 2 and n
 *            even) or (n odd and n mod 8 == 5 or 7), otherwise δ(n) = 0. This
 *            yields the OEIS sequence A243205
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute floor((n² + 4n + 2)/4) */
    seq_t total = nn * nn + (seq_t)4 * nn + (seq_t)2;
    seq_t res   = total / (seq_t)4;

    bool add = false;
    if (n == 2) {
        add = false;                /* special case for n = 2 : a(2)=3 */
    } else if ((n & 1) == 0) {      /* n even, n > 2 */
        add = true;
    } else {                        /* n odd */
        idx_t mod8 = n % 8;
        if (mod8 == 5 || mod8 == 7) {
            add = true;
        }
    }

    if (add) {
        res += (seq_t)1;
    }
    return res;
}

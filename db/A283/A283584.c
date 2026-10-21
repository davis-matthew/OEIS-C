/*
 * A283584  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 646", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A283584
   Formula: a(0)=1, a(1)=3, a(2)=5, a(3)=11, a(4)=21, a(5)=43, a(6)=69,
            a(7)=219, a(8)=261, a(9)=795, a(10)=1301, a(11)=2923,
   a(n)=4*a(n‑2)+a(n‑8)–4*a(n‑10)  for n>11
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Memoised cache of previously computed terms. */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  size   = 0;          /* number of values already present */

    /* Grow cache if necessary (doubling strategy). */
    if ((size_t)n >= cap) {
        size_t new_cap = ((size_t)n + 1) * 2UL;
        if (new_cap == 0) new_cap = 1;          /* never zero */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Initial values for indices 0 … 11. */
    static const seq_t init[12] = {
        (seq_t)1,   (seq_t)3,   (seq_t)5,   (seq_t)11,
        (seq_t)21,  (seq_t)43,  (seq_t)69,  (seq_t)219,
        (seq_t)261, (seq_t)795, (seq_t)1301, (seq_t)2923
    };

    /* Compute any missing terms up to n. */
    for (size_t i = size; i <= (size_t)n; ++i) {
        if (i < 12) {
            cache[i] = init[i];
        } else {
            cache[i] = 4 * cache[i - 2] + cache[i - 8] - 4 * cache[i - 10];
        }
    }

    size = (size_t)n + 1;
    return cache[(size_t)n];
}

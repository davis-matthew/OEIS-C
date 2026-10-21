/*
 * A273181  The F_4-Eulerian numbers.
 * https://oeis.org/A273181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = # { w∈W(F₄) | |descent_set(w)| = n }. For the Weyl group
 *            of type F₄ the descent set size ranges from 0 to 4, giving the
 *            values 1, 236, 678, 236, 1; all other n yield 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* All values for n>4 or n<0 (impossible for unsigned idx_t) are zero. */
    if (n > (idx_t)4)
        return (seq_t)0;

    switch ((uint64_t)n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)236;
        case 2: return (seq_t)678;
        case 3: return (seq_t)236;
        case 4: return (seq_t)1;
        default: return (seq_t)0; /* unreachable, keeps type safety */
    }
}

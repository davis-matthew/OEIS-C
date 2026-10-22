/*
 * A369876  Square array read by descending antidiagonals: For T(n,k), define
 *          sequence {b(i)} where b(0) = n, b(1) = k, b(i) = A000265(b(i-1) +
 *          b(i-2)). T(n,k) is the number of steps until reaching the cyclic
 *          part of {b(i)}.
 * https://oeis.org/A369876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(n0,k0) where (n0,k0) is the pair indexed by n in the
 *            square array read by descending antidiagonals. T(n,k) is the
 *            number of steps until the sequence b(i) defined by b(0)=n,
 *            b(1)=k, b(i)=odd_part(b(i-1)+b(i-2)) enters a triple
 *            b(i)=b(i+1)=b(i+2)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/odd_part.h"

static seq_t T_steps(idx_t n0, idx_t k0)
{
    seq_t b0 = (seq_t)n0;
    seq_t b1 = (seq_t)k0;
    seq_t b2 = odd_part(b0 + b1);
    if (b0 == b1 && b1 == b2)    /* triple at i = 0 */
        return (seq_t)0;

    seq_t prevprev = b0;
    seq_t prev     = b1;
    seq_t curr     = b2;
    seq_t next;
    idx_t i = 1;                  /* current index of possible triple start */

    for (;; ++i) {
        next = odd_part(curr + prev);
        if (prev == curr && curr == next)   /* triple found */
            return (seq_t)i;
        /* shift window: prevprev→prev, prev→curr, curr→next */
        prevprev = prev;
        prev     = curr;
        curr     = next;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* not defined for n=0; return 0 as safe fallback */

    /* Find minimal s such that s*(s-1)/2 >= n.  For n <= 10⁴, s <= 150,
       so a simple loop is fast. */
    idx_t s = 2;
    while ((s * (s - 1)) / 2 < n)
        ++s;

    /* Cumulative count before sum s */
    idx_t before = ((s - 1) * (s - 2)) / 2;

    /* Zero‑based offset within this antidiagonal */
    idx_t offset = n - before - 1;

    /* Convert to (n0,k0) pair: n0 = offset+1, k0 = s - n0 */
    idx_t n0 = offset + 1;
    idx_t k0 = s - n0;

    return T_steps(n0, k0);
}

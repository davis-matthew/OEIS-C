/*
 * A250740  Number of (n+1) X (6+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nonincreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) + 126
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^e with the given seq_t type.  Works for built‑in
 * integer types as well as arbitrary‑precision types provided in
 * the build. */
static seq_t pow2_seq(idx_t e)
{
    seq_t r = 1;                /* start with 2^0 */
    for (idx_t i = 0; i < e; ++i)
        r += r;                 /* doubling: r = 2*r */
    return r;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition.  Compute 2^(n+1) + 126.  */
    seq_t t = pow2_seq(n + 1);
    return t + (seq_t)126;
}

/*
 * A259669  a(0)=0, a(1)=1, a(n) = min{5 a(k) + (5^(n-k)-1)/4, k=0..(n-1)} for
 *          n>=2.
 * https://oeis.org/A259669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(1)=1; a(n) = min{5 a(k) + (5^(n-k)-1)/4, 0 ≤ k < n}
 *            Closed form: a(n) = Σ_{i=0}^{n-1} 5^{⌊(√(8i+1)-1)/2⌋}
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 91, u64 n <= 356,
 *            u128 n <= 1448, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum can be computed iteratively by keeping the current block
 * exponent e and its block size (e+1).  The value 5^e is maintained
 * as a running product.  No dynamic allocation is required, so the
 * implementation works for both builtin integer types and for
 * arbitrary‑precision integer classes that support the normal
 * operators. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum   = (seq_t)0;   /* running total */
    seq_t pow   = (seq_t)1;   /* current power 5^e, e starts at 0 */
    idx_t block = 1;          /* size of the current block (e+1) */
    idx_t pos   = 1;          /* position within the block (1‑based) */

    for (idx_t i = 0; i < n; ++i) {
        sum += pow;          /* add current term 5^e */
        ++pos;
        if (pos > block) {   /* finished this block, move to next exponent */
            pow   = pow * (seq_t)5;   /* 5^(e+1) */
            ++block;                 /* next block size is (e+2) */
            pos = 1;
        }
    }
    return sum;
}

/*
 * A246105  Least m > 0 for which (s(m),...,s(n+m-1)) = (s(n),...,s(0)), the
 *          reverse of the first n+1 terms of the infinite Fibonacci word
 *          A003849.
 * https://oeis.org/A246105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   concatenation of descending Fibonacci blocks (F(k), F(k-1), ...,
 *            F(3), F(2)) for k ≥ 3. Each block has length F(k) and contains
 *            the numbers F(k), F(k)-1, … ,1. The sequence a(n) is the n-th
 *            element of the infinite concatenation
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* External types provided by the build environment:
 *  - idx_t is an unsigned integer type for indices (e.g., uint64_t)
 *  - seq_t is an unsigned integer type large enough to hold all values
 *    needed for the sequence (uint8_t ... unsigned __int128, or an
 *    arbitrary‑precision integer).
 */
extern idx_t _floor_isqrt_u64(idx_t n);  /* not used but allowed */

/* Compute a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic with seq_t values. */
    seq_t sn = (seq_t)n;

    /* Fibonacci numbers F(1)=1, F(2)=1.  We start generating from F(3). */
    seq_t f_prev = (seq_t)1;  /* F(k-2)   */
    seq_t f_curr = (seq_t)1;  /* F(k-1)   */

    seq_t cumulative = (seq_t)0;   /* Sum of block lengths processed so far. */

    for (idx_t k = 3;; ++k)
    {
        /* Compute F(k) = F(k-1) + F(k-2). */
        seq_t f_k = f_prev + f_curr;

        /* If the block of length f_k would exceed the target index,
           the desired element lies in this block.  We compute its
           offset within the block and return F(k) - offset. */
        if (cumulative + f_k > sn)
        {
            seq_t offset = sn - cumulative;   /* 0 ≤ offset < f_k */
            return f_k - offset;              /* descending sequence */
        }

        /* Move to the next Fibonacci number. */
        cumulative += f_k;
        f_prev = f_curr;
        f_curr = f_k;
    }
}

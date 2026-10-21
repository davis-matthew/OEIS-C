/*
 * A249727  Start with a(1) = 1; then numbers 1 .. primepi(2), followed by
 *          numbers 1 .. primepi(3), and then numbers 1 .. primepi(4), ...,
 *          etc, where A000720 gives primepi.
 * https://oeis.org/A249727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9874, u16 n <= 9874, u32 n <= 9874, u64 n <= 9874,
 *            u128 n <= 9874, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A249727: a(1)=1; thereafter concatenate the lists
   1..π(2), 1..π(3), 1..π(4), …
   For n ≥ 2, let cum be the total number of terms produced so far
   (initially cum = 1 for a(1)).  For each m ≥ 2, compute π(m) and
   check if cum + π(m) ≥ n.  If so, the desired term is the
   position n − cum inside that block, i.e. 1 ≤ a(n) ≤ π(m).
   Otherwise add π(m) to cum and continue.
*/
#include "../utils/is_prime.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t cum = 1;        /* 1 + sum of lengths of processed blocks */
    idx_t pi_count = 0;   /* π(current m‑1) */

    for (idx_t m = 2;; ++m)
    {
        if (is_prime(m)) ++pi_count;   /* now π(m) */
        idx_t block_len = pi_count;    /* π(m) */
        if (cum + block_len >= n)
        {
            idx_t pos = n - cum;       /* 1‑based position inside block */
            return (seq_t)pos;
        }
        cum += block_len;              /* move to next block */
    }
}

/*
 * A266323  Binary representation of the n-th iteration of the "Rule 19"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Common formula: a(0)=1, a(1)=101, a(n)=0 for even n>0,
   for odd n≥3, a(n) is decimal all‑ones of length L=4*((n/2)+1)-1,
   i.e. a(n) = (10^L-1)/9. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)101;
    if ((n & 1u) == 0u)      /* even n > 0 */
        return (seq_t)0;

    /* odd n ≥ 3 */
    idx_t k = (n >> 1u) + 1u;          /* (n/2)+1   */
    idx_t L = 4u * k - 1u;             /* desired length of the all‑ones string */

    /* For small L we can safely compute 10^L by simple multiplication. */
    if (L <= 19u) {
        seq_t val = 1;
        for (idx_t i = 0; i < L; ++i)
            val *= (seq_t)10;
        val -= (seq_t)1;
        val /= (seq_t)9;
        return val;
    } else {
        /* For larger L use the generic big_pow utility (works with arbitrary‑precision seq_t). */
        seq_t val = big_pow((seq_t)10, (seq_t)L);
        val -= (seq_t)1;
        val /= (seq_t)9;
        return val;
    }
}

/*
 * A320028  a(n) is the first prime encountered when running the Collatz
 *          algorithm (halving and tripling steps) on the number n.
 * https://oeis.org/A320028
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 6302, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

/* Search algorithm for OEIS A320028:
   returns the first prime encountered in the halving‑tripling Collatz
   sequence starting at n (for n ≥ 2). */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* defined only for n ≥ 2; value below offset is irrelevant */

    seq_t x = (seq_t)n;
    while (1) {
        if (is_prime_big(x))
            return x;
        if ((x & 1) == 0)
            x >>= 1;       /* even → divide by 2 */
        else
            x = x * 3 + 1; /* odd → 3*x + 1 */
    }
}

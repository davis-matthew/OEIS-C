/*
 * A377413  a(n) is the least term of A126684 of the form k*n with k > 0.
 * https://oeis.org/A377413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least k*n with k>0 such that k*n ∈ A126684
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 3070, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_A126684
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126684 consists of all integers whose binary representation has
   all set bits at positions of the same parity (either all even
   or all odd). */
#include <stdbool.h>
#include "../utils/in_A126684.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t n_seq = (seq_t)n;          /* work in seq_t for arithmetic */
    seq_t candidate = n_seq;         /* start with k = 1 */

    /* Find the smallest multiple of n that lies in A126684. */
    while (true) {
        if (in_A126684(candidate))
            return candidate;
        candidate += n_seq;
    }
}

/*
 * A309027  Prime powers of the form 12*c^2 + 4*c + 3, where c is an arbitrary
 *          integer.
 * https://oeis.org/A309027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309027: Prime powers of 12*c^2 + 4*c + 3, where c is an integer.
   The terms are obtained by enumerating c as 0, -1, 1, -2, 2, -3, 3, …,
   testing each value for being a prime power.
   For the ranges considered all such values are (conjecturally) prime,
   so the code tests primality only. */
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Return the n-th term, with OEIS offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return safe value */
        return (seq_t)0;
    if (n == 1)          /* first term is 3, when c = 0 */
        return (seq_t)3;

    idx_t count = 1;     /* we already have the first prime 3 */
    idx_t k = 1;         /* start with |c| = 1 */

    while (1) {
        seq_t kt = (seq_t)k;

        /* c = -k : 12*k^2 - 4*k + 3 */
        seq_t val_neg = ((seq_t)12) * kt * kt - ((seq_t)4) * kt + ((seq_t)3);
        if (is_prime_big(val_neg)) {
            ++count;
            if (count == n)
                return val_neg;
        }

        /* c = +k : 12*k^2 + 4*k + 3 */
        seq_t val_pos = ((seq_t)12) * kt * kt + ((seq_t)4) * kt + ((seq_t)3);
        if (is_prime_big(val_pos)) {
            ++count;
            if (count == n)
                return val_pos;
        }

        ++k;
    }
}

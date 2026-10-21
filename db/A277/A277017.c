/*
 * A277017  Number of maximal runs of 1-bits (in binary expansion of n) such
 *          that the length of run >= A000040(1 + the total number of zeros to
 *          the right of that run).
 * https://oeis.org/A277017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of maximal runs of 1-bits (in binary expansion of
 *            n) such that the length of run >= A000040(1 + the total number
 *            of zeros to the right of that run)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t bits = n;          /* working copy of the binary representation */
    seq_t count = 0;         /* result to return */
    idx_t zeros = 0;         /* number of zeros seen in less significant bits */

    while (bits != (seq_t)0) {
        /* If the least significant bit is zero, record it and continue */
        if ((bits & (seq_t)1) == (seq_t)0) {
            zeros++;
            bits >>= 1;
        } else {
            /* Found the start of a maximal run of 1-bits */
            seq_t run_len = 0;
            while ((bits & (seq_t)1) != (seq_t)0) {
                run_len++;
                bits >>= 1;
            }
            /* zeros to the right of this run are the zeros seen so far */
            seq_t prime_needed = (seq_t)get_prime((idx_t)zeros + 1);
            if (run_len >= prime_needed)
                count++;
            /* no reset of zeros; the zeros counted already belong
               to less significant bits of the number */
        }
    }

    return count;
}

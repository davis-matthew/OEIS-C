/*
 * A347202  Numbers whose number of odd divisors is not equal to 2.
 * https://oeis.org/A347202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer whose odd part is not a prime
 *            (exclude numbers of the form 2^k * p with p an odd prime)
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;
    while (1) {
        idx_t odd = candidate;
        while ((odd & 1) == 0) odd >>= 1;          /* strip powers of two */
        if (odd == 1 || !is_prime(odd)) {          /* odd part is 1 or composite */
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate++;
    }
}

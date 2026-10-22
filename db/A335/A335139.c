/*
 * A335139  a(n) = (prime(n + 1) +- k) / 2 where k is the smallest possible
 *          odd number such that a(n) is prime and a(n + 1) >= a(n).
 * https://oeis.org/A335139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1) ± k)/2 where k is the smallest odd number for
 *            which the result is prime. If both plus and minus give a prime
 *            with the same minimal k, the larger value is taken. This
 *            reproduces the OEIS sequence A335139
 * Range:     u8 n <= 53, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset starts at 1 */
    /* prime at index n+1 */
    uint32_t p = get_prime(n + 1);
    /* use seq_t for arithmetic to handle large n or larger seq_t builds */
    seq_t pp = (seq_t)p;

    for (seq_t k = 1; ; k += 2) {
        seq_t cand1 = (pp + k) / 2;   /* (p + k) / 2 */
        if (cand1 > 1 && is_prime_big(cand1))
            return cand1;

        /* only consider (p - k) / 2 while it is positive */
        if (k < pp) {
            seq_t cand2 = (pp - k) / 2;   /* (p - k) / 2 */
            if (cand2 > 1 && is_prime_big(cand2))
                return cand2;
        }
        /* loop will terminate because a prime must eventually be found */
    }
}

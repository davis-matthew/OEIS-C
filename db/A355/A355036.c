/*
 * A355036  a(n) is the least number whose product of digits in primorial base
 *          equals n.
 * https://oeis.org/A355036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 100,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the least primorial‑base number whose digits product equals n */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Remaining product of digits that must be achieved */
    seq_t rem = (seq_t)n;
    /* Resulting primorial‑base number */
    seq_t value = (seq_t)0;
    /* Weight of the current position (product of all smaller primes) */
    seq_t weight = (seq_t)1;
    /* Index of the current prime (starting from 2) */
    idx_t pos = 0U;

    /* Greedily assign the largest possible digit at each position that
       divides the remaining product. This produces the smallest possible
       primorial‑base value. */
    while (rem > (seq_t)1) {
        uint32_t prime = get_prime(pos + 1U);     /* prime_{pos+1} */
        seq_t   limit = (seq_t)(prime - 1U);      /* allowed digit range */

        /* Find the largest divisor of rem that does not exceed limit. */
        seq_t d = (seq_t)1;
        for (uint32_t cand = (uint32_t)limit; cand >= 1U; --cand) {
            seq_t cand_seq = (seq_t)cand;
            if (rem % cand_seq == (seq_t)0) {
                d = cand_seq;
                break;
            }
        }

        /* Record the chosen digit and update the remaining product. */
        value += d * weight;
        rem   /= d;

        /* Prepare for the next position. */
        weight *= (seq_t)prime;
        ++pos;
    }

    return value;
}

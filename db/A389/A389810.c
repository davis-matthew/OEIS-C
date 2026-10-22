/*
 * A389810  Prime(k)-1 copies of prime(k) ordered by increasing k.
 * https://oeis.org/A389810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the prime p_k such that Σ_{i=1}^{k-1} (prime(i)-1) < n ≤
 *            Σ_{i=1}^{k} (prime(i)-1) (OEIS A389810)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, guard against misuse */

    idx_t cumulative = 0;           /* total number of terms seen so far */
    idx_t candidate   = 2;           /* current integer to test for primality */

    while (1) {
        if (is_prime(candidate)) {      /* candidate is prime */
            idx_t repeats = candidate - 1;   /* how many times prime appears */
            if (cumulative + repeats >= n) {
                return (seq_t)candidate;     /* this prime covers position n */
            }
            cumulative += repeats;           /* move past this block */
        }
        ++candidate;                      /* advance to next integer */
    }
}

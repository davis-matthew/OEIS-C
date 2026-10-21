/*
 * A261085  Number of steps needed to reach zero when starting from the n-th
 *          prime [i.e., setting k to A000040(n)] and repeatedly applying the
 *          map that replaces k with k - d(k), where d(k) is the number of
 *          divisors of k (A000005).
 * https://oeis.org/A261085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A155043(A000040(n)) * computed by repeatedly subtracting
 *            the divisor count from the n‑th prime until zero
 * Range:     u8 n <= 241, u16 n <= 6543, u32 n <= 6543, u64 n <= 6543,
 *            u128 n <= 6543, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"          /* int get_prime(idx_t k); */
#include "../utils/divisor_count_u64.h"  /* uint64_t divisor_count_u64(uint64_t n); */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; a(1) corresponds to the first prime. */
    if (n == 0) return (seq_t)0;          /* undefined for offset < 1 */

    /* Retrieve the n‑th prime (1‑based).  The returned value fits in 32 bits. */
    uint32_t p = get_prime(n);

    uint64_t k = (uint64_t)p;            /* current value */
    seq_t   steps = (seq_t)0;            /* count of subtractions */

    while (k > 0) {
        uint64_t d = divisor_count_u64(k);  /* number of divisors of k */
        k -= d;                            /* step */
        ++steps;                           /* increment step count */
    }

    return steps;
}

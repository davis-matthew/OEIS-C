/*
 * A308772  Sum of the third largest parts of the partitions of n into 4 prime
 *          parts.
 * https://oeis.org/A308772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the third largest parts of all partitions of n
 *            into exactly 4 prime parts
 * Range:     u8 n <= 55, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A308772 for a given n. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 8) return (seq_t)0;   /* no partition exists with 4 prime parts for n < 8 */

    /* build list of primes up to n */
    size_t limit = (size_t)n;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)0; /* allocation failed, return 0 */

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t mult = p * p; mult <= limit; mult += p)
                is_prime[mult] = false;
        }
    }

    /* count primes and store them in an array */
    size_t prime_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_count;

    uint32_t *primes = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!primes) { free(is_prime); return (seq_t)0; }

    size_t pi = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[pi++] = (uint32_t)i;

    /* now iterate over p4 <= p3 <= p2 <= p1 such that p1+p2+p3+p4 = n */
    seq_t total = 0;
    for (size_t ip4 = 0; ip4 < prime_count; ++ip4) {
        uint32_t p4 = primes[ip4];
        if (p4 > n / 4) break;  /* p4 cannot exceed n/4 */
        for (size_t ip3 = ip4; ip3 < prime_count; ++ip3) {
            uint32_t p3 = primes[ip3];
            if (p3 > (n - p4) / 3) break;
            for (size_t ip2 = ip3; ip2 < prime_count; ++ip2) {
                uint32_t p2 = primes[ip2];
                if (p2 > (n - p4 - p3) / 2) break;
                uint32_t p1 = (uint32_t)(n - p4 - p3 - p2);
                if (p1 < p2) continue;             /* require p1 >= p2 */
                if (p1 <= limit && is_prime[p1]) {  /* ensure p1 is prime */
                    total += (seq_t)p3;           /* add the third largest part */
                }
            }
        }
    }

    free(primes);
    free(is_prime);
    return total;
}

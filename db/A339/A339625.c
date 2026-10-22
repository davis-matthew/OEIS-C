/*
 * A339625  a(n) is the number of ways to write 6*n = p + q with p a lesser
 *          twin prime (A001359) and q a greater twin prime (A006512).
 * https://oeis.org/A339625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 6*n = p + q where p is a (lesser)
 *            twin prime (p and p+2 are prime) and q is a (greater) twin prime
 *            ((q-2) and q are prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, just in case */

    /* 6*n is the target sum */
    idx_t limit = 6 * n;

    /* Allocate an array for primality testing */
    bool *is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_prime) return 0;        /* allocation failure → return 0 */

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_prime[i]) continue;
        for (idx_t j = i * i; j <= limit; j += i) {
            is_prime[j] = false;
        }
    }

    /* Count representations */
    seq_t count = 0;
    for (idx_t p = 2; p + 2 <= limit; ++p) {
        if (is_prime[p] && is_prime[p + 2]) {          /* p is a lower twin prime */
            idx_t q = limit - p;                      /* candidate greater twin prime */
            if (q >= 3 && q <= limit &&                /* within bounds */
                is_prime[q] && is_prime[q - 2]) {     /* q is a greater twin prime */
                count++;
            }
        }
    }

    free(is_prime);
    return count;
}

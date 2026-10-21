/*
 * A291564  Number of partitions of 2n into two parts such that one part is
 *          prime and the other is nonprime.
 * https://oeis.org/A291564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = n - Σ_{i=1..n} [prime(i) = prime(2n-i)]
 * Range:     u8 n <= 1023, u16 n <= 8551, u32 n <= 8551, u64 n <= 8551,
 *            u128 n <= 8551, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* sieve bitmap: prime_flag[x] == 1 iff x is prime, else 0 */
static uint8_t *prime_flag = NULL;
static idx_t max_sieve = 0;

/*   ensure_sieve(limit) builds/expands the sieve up to at least `limit` */
static void ensure_sieve(idx_t limit)
{
    if (limit <= max_sieve) return;

    /* Allocate a new bitmap (size limit+1, indices 0..limit) */
    uint8_t *new_flag = (uint8_t *)malloc((size_t)(limit + 1));
    if (!new_flag) return;                     /* allocation failure → caller must handle */

    memset(new_flag, 1, (size_t)(limit + 1));
    if (limit >= 0) new_flag[0] = 0;
    if (limit >= 1) new_flag[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_flag[p]) {
            for (idx_t j = p * p; j <= limit; j += p) {
                new_flag[j] = 0;
            }
        }
    }

    /* Replace the old bitmap */
    free(prime_flag);
    prime_flag = new_flag;
    max_sieve = limit;
}

/* primality test that first uses the sieve, if available */
static inline bool local_is_prime(idx_t x)
{
    if (x <= max_sieve) return prime_flag[x];
    return is_prime(x);
}

/* sequence function.  For n == 0 we simply return 0 (offset not defined). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offset starts at 1 */

    idx_t limit = 2 * n;
    ensure_sieve(limit);

    seq_t mismatches = 0;

    for (idx_t i = 1; i <= n; ++i) {
        bool pi = local_is_prime(i);
        bool pj = local_is_prime(limit - i);
        if (pi != pj) ++mismatches;
    }

    return mismatches;
}

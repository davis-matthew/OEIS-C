/*
 * A330936  Number of nontrivial factorizations of n into factors > 1.
 * https://oeis.org/A330936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n * into factors >
 *            1, minus 2 for the trivial factorizations * (the singleton [n]
 *            and the prime‑factorization)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

/* Count unordered factorizations of n with each factor >= min_factor.
 * The first count (cnt = 1) corresponds to the partition [n] itself. */
static seq_t
count_factors(uint64_t n, uint64_t min_factor)
{
    seq_t cnt = 1;                           /* [n] itself */
    uint64_t limit = int_sqrt(n);            /* floor(sqrt(n)) */
    for (uint64_t d = min_factor; d <= limit; ++d) {
        if (n % d == 0) {
            cnt += count_factors(n / d, d);   /* recurse with the remainder */
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n <= 1)          return 0;          /* offset 1, a(1)=0 */
    if (is_prime(n))     return 0;          /* primes have no nontrivial factorizations */
    seq_t total = count_factors((uint64_t)n, 2);
    return total - 2;                       /* remove [n] and its prime factorisation */
}

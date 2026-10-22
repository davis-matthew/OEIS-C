/*
 * A394056  Number of positive integers <= n with 4 divisors.
 * https://oeis.org/A394056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers k ≤ n with exactly 4 divisors
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_cube_root, int_sqrt, upper_bound_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided helper functions:
 *   - build_primes_upto(limit, &primes, &cnt)
 *   - int_sqrt(n)
 *   - floor_cube_root(x)  // operates on seq_t
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"
#include "../utils/floor_cube_root.h"
#include "../utils/upper_bound_primes.h"

static uint64_t *primes = NULL;     /* array of primes up to current_limit */
static size_t     primes_cnt = 0;   /* number of primes stored */
static idx_t      current_limit = 0;/* maximum n for which primes are stored */

seq_t a(idx_t n)
{
    /* For the first few n the answer is 0 */
    if (n < 6)
        return (seq_t)0;

    /* Build or extend the prime table up to n if needed */
    if (n > current_limit) {
        free(primes);                       /* free old array if any */
        build_primes_upto(n, &primes, &primes_cnt);
        current_limit = n;
    }

    /* Count cubes: primes p with p^3 ≤ n */
    idx_t cube_root = (idx_t)floor_cube_root((seq_t)n); /* seq_t namespace */
    size_t cube_cnt = upper_bound_primes(primes, primes_cnt,
                                         (uint64_t)cube_root);

    /* Count products of two distinct primes p < q with pq ≤ n */
    idx_t sqrt_n    = int_sqrt(n);
    size_t sqrt_cnt = upper_bound_primes(primes, primes_cnt,
                                         (uint64_t)sqrt_n);

    seq_t total = (seq_t)cube_cnt;
    for (size_t i = 0; i < sqrt_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t limit = n / p;
        if (limit <= p)            /* further p will only increase */
            break;

        size_t up_to_limit = upper_bound_primes(primes, primes_cnt,
                                                limit);
        seq_t add = (seq_t)(up_to_limit - (i + 1)); /* only q > p */
        total += add;
    }

    return total;
}

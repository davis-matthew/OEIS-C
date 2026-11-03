/*
 * A394056  Number of positive integers <= n with 4 divisors.
 * https://oeis.org/A394056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers k ≤ n with exactly 4 divisors
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9832 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32, isqrt
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
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/isqrt.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return the index of the first prime > m.
 * i.e., the number of primes ≤ m. */
static size_t upper_bound_primes(const uint64_t *pr, size_t cnt, uint64_t m)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (pr[mid] <= m)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

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
    idx_t sqrt_n    = isqrt(n);
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

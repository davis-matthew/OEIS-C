/*
 * A308512  Sum of the smallest side lengths of all integer-sided triangles
 *          with perimeter n whose smallest side length is prime.
 * https://oeis.org/A308512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of the smallest side lengths of all integer-sided triangles
 *            with perimeter n whose smallest side length is prime
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static state for prime generation */
static bool *sieve = NULL;
static idx_t sieve_limit = 0;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Ensure that all primes up to `limit` are generated. */
static void ensure_primes_up_to(idx_t limit) {
    if (limit <= sieve_limit) return;

    /* Rebuild the sieve and prime list from scratch up to `limit`. */
    free(sieve);
    sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= limit; ++i) sieve[i] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = true;
        }
    }

    free(prime_list);
    /* Rough upper bound for number of primes: limit/2 for small limits. */
    size_t est = (limit >= 2) ? (size_t)(limit / 2) : 0;
    prime_list = (uint32_t *)malloc(est * sizeof(uint32_t));
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) prime_list[cnt++] = (uint32_t)i;
    }
    prime_count = cnt;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_count * sizeof(uint32_t));
    if (tmp) prime_list = tmp;

    sieve_limit = limit;
}

seq_t a(idx_t n) {
    /* For n < 6 no valid integer triangle with a prime smallest side exists. */
    if (n < 6) return (seq_t)0;

    idx_t max_a = n / 3;                      /* largest possible smallest side */
    if (max_a < 2) return (seq_t)0;           /* no prime <= 1 */

    ensure_primes_up_to(max_a);

    seq_t sum = (seq_t)0;

    for (size_t pi = 0; pi < prime_count && prime_list[pi] <= max_a; ++pi) {
        idx_t a_side = (idx_t)prime_list[pi];
        idx_t S = n - a_side;                 /* b + c = S */

        /* upper bound for b: floor((n - a)/2) */
        idx_t upper = S / 2;

        /* lower bound from two constraints */
        idx_t lower2 = 0;                      /* floor((n - 2a)/2) + 1 */
        if (n >= 2 * a_side) {
            lower2 = ((n - 2 * a_side) / 2) + 1;
        }
        idx_t lower = (a_side > lower2) ? a_side : lower2;

        if (lower > upper) continue;          /* no feasible b */
        seq_t count = (seq_t)(upper - lower + 1); /* number of triangles with this a */

        sum += (seq_t)a_side * count;
    }

    return sum;
}

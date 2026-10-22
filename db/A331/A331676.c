/*
 * A331676  Ceiling of circumradius of triangle with consecutive prime sides.
 * https://oeis.org/A331676
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A331676: ceil of circumradius of triangle with consecutive prime sides. */
/* Computation performed by integer inequality and binary search. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Internal static cache of primes (first 19k primes up to 200000). */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

/* Initialise the prime cache on first use. */
static void init_primes(void)
{
    if (primes != NULL) return;

    const size_t limit = 200000;          /* large enough for n ≤ 10 000 */
    bool *is_comp = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_comp) return;                 /* allocation failure unlikely */

    for (size_t i = 0; i <= limit; ++i) is_comp[i] = false;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (size_t k = p * p; k <= limit; k += p) {
                is_comp[k] = true;
            }
        }
    }

    /* Count primes and fill array */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) if (!is_comp[i]) count++;
    primes = (uint64_t*)malloc(count * sizeof(uint64_t));
    if (!primes) { free(is_comp); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) if (!is_comp[i]) primes[idx++] = i;
    primes_cnt = count;

    free(is_comp);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Undefined for n < 2; behaviour not important for the tests. */
    if (n < 2) return (seq_t)0;

    init_primes();
    if (primes == NULL) return (seq_t)0;
    if ((size_t)(n + 2) > primes_cnt) return (seq_t)0;

    uint64_t a1 = primes[(size_t)n - 1];
    uint64_t a2 = primes[(size_t)n];
    uint64_t a3 = primes[(size_t)n + 1];

    /* x1 = a+b+c, x2 = b+c-a, x3 = c+a-b, x4 = a+b-c (all odd integers) */
    uint64_t x1 = a1 + a2 + a3;
    uint64_t x2 = a2 + a3 - a1;
    uint64_t x3 = a3 + a1 - a2;
    uint64_t x4 = a1 + a2 - a3;

    /* Denominator D = x1*x2*x3*x4 (fits in 128 bits) */
    __uint128_t D = (__uint128_t)x1 * x2 * x3 * x4;

    /* Numerator N = a1^2 * a2^2 * a3^2 (fits in 128 bits) */
    __uint128_t a1sq = (__uint128_t)a1 * a1;
    __uint128_t a2sq = (__uint128_t)a2 * a2;
    __uint128_t a3sq = (__uint128_t)a3 * a3;
    __uint128_t N = a1sq * a2sq;
    N = N * a3sq;

    /* Binary search for minimal m with m^2 * D >= N */
    uint64_t low = 1;
    uint64_t high = x1;                /* a+b+c is a safe upper bound */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        __uint128_t lhs = (__uint128_t)mid * mid * D;
        if (lhs >= N) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)low;
}

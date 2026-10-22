/*
 * A378367  Greatest non prime power <= n, allowing 1.
 * https://oeis.org/A378367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest non prime power <= n, 1 allowed
 * Range:     u8 n <= 63, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* Helper: cache primes up to a certain limit */
static uint64_t *PRIMES = NULL;
static size_t PRIME_COUNT = 0U;
static uint64_t PRIME_UPTO = 0U;

static void ensure_primes_upto(uint64_t limit)
{
    if (PRIME_UPTO >= limit) return;

    uint64_t *tmp = NULL;
    size_t cnt = 0U;
    build_primes_upto(limit, &tmp, &cnt);

    free(PRIMES);
    PRIMES = tmp;
    PRIME_COUNT = cnt;
    PRIME_UPTO = limit;
}

/* Helper: test whether n (uint64_t) is a prime power (>1) */
static int is_prime_power(uint64_t n)
{
    if (n <= 1U) return 0;                     /* 1 is not a prime power */
    if (isPrimeTrial(n)) return 1;             /* primes are prime powers */

    uint64_t r = (uint64_t)floor_sqrt((seq_t)n);
    ensure_primes_upto(r);

    for (size_t i = 0U; i < PRIME_COUNT; ++i) {
        uint64_t p = PRIMES[i];
        if (p > r) break;

        seq_t power = (seq_t)p * (seq_t)p;
        if (power > (seq_t)n) continue;

        while (power <= (seq_t)n) {
            if (power == (seq_t)n) return 1;
            if (power > (seq_t)n / (seq_t)p) break;
            power *= (seq_t)p;
        }
    }
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)0;
    for (idx_t m = n; ; --m) {
        if (!is_prime_power((uint64_t)m))
            return (seq_t)m;
    }
}

/*
 * A348283  Numbers that are multiples of their arithmetic derivative,
 *          A003415.
 * https://oeis.org/A348283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4114, u32 n <= 9743, u64 n <= 9743,
 *            u128 n <= 9743, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348283: numbers that are multiples of their arithmetic derivative.
   The terms (after the initial 0) are the primes and the prime powers p^p,
   sorted increasingly. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility: compute p^p up to a limit (uint64_t). */
static uint64_t power_limit(uint32_t p, uint64_t limit)
{
    uint64_t val = 1ULL;
    for (unsigned e = 1; e <= p; ++e) {
        if (val > limit / p) return limit + 1;   /* overflow / too big */
        val *= p;
    }
    return val;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;                /* a(1)=0 */

    /* Number of terms needed after the initial 0. */
    idx_t target = n - 1;

    /* Upper bound that safely contains at least target primes for n <= 10000.
       2,000,000 gives ~148,933 primes and all small p^p values.          */
    static const idx_t LIMIT = 2000000ULL;

    /* Static data structures, initialized on first call. */
    static bool *is_composite = NULL;
    static uint32_t *primes = NULL;
    static size_t prime_count = 0;

    static seq_t *prime_powers = NULL;
    static size_t power_count = 0;

    if (!is_composite) {
        /* Allocate sieve array. */
        is_composite = (bool*)calloc(LIMIT + 1, sizeof(bool));
        if (!is_composite) return 0; /* allocation failure */

        /* Sieve primes. */
        primes = (uint32_t*)malloc((LIMIT / 10) * sizeof(uint32_t));
        if (!primes) return 0;

        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (!is_composite[i]) {
                primes[prime_count++] = (uint32_t)i;
                if ((uint64_t)i * i <= LIMIT) {
                    for (idx_t j = i * i; j <= LIMIT; j += i)
                        is_composite[j] = true;
                }
            }
        }

        /* Compute prime powers p^p that are <= LIMIT. */
        power_count = 0;
        prime_powers = (seq_t*)malloc(prime_count * sizeof(seq_t));
        if (!prime_powers) return 0;

        for (size_t idx = 0; idx < prime_count; ++idx) {
            uint32_t p = primes[idx];
            uint64_t val = power_limit(p, LIMIT);
            if (val <= LIMIT) {
                prime_powers[power_count++] = (seq_t)val;
            }
        }
        /* prime_powers is already sorted because p^p increases with p. */
    }

    /* Merge the two sorted lists: primes and prime powers. */
    idx_t count = 0;
    size_t p_i = 0, pp_i = 0;
    seq_t answer = 0;

    while (count < target) {
        seq_t next;
        if (p_i < prime_count &&
            (pp_i >= power_count || (seq_t)primes[p_i] <= prime_powers[pp_i])) {
            next = (seq_t)primes[p_i++];
        } else {
            next = prime_powers[pp_i++];
        }
        answer = next;
        ++count;
    }

    return answer;
}

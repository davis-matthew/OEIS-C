/*
 * A294280  a(n) = least positive k such that omega(n+k) > max(omega(n),
 *          omega(k)), where omega(m) = A001221(m), the number of distinct
 *          primes dividing m.
 * https://oeis.org/A294280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for minimal k such that ω(n+k) > max(ω(n),ω(k)),
   where ω(m) = number of distinct prime divisors of m. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------- Prime sieve for ω ----------------- */
static uint8_t *omega_table = NULL;   /* array storing ω values */
static size_t omega_limit = 0;
static bool omega_ready = false;

static void init_omega_sieve(size_t limit)
{
    omega_table = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!omega_table) abort();

    for (size_t i = 2; i <= limit; ++i) {
        if (omega_table[i] == 0) {          /* prime detected */
            for (size_t j = i; j <= limit; j += i)
                omega_table[j] = (uint8_t)(omega_table[j] + 1);
        }
    }
    omega_limit = limit;
    omega_ready = true;
}

static void ensure_omega_limit(size_t needed)
{
    if (needed <= omega_limit) return;

    size_t new_limit = omega_limit > 0 ? omega_limit : 1;
    while (new_limit < needed) {
        new_limit *= 2;
    }

    /* Rebuild sieve from scratch up to new_limit */
    free(omega_table);
    init_omega_sieve(new_limit);
}

/* ----------------- Main sequence function ----------------- */
seq_t a(idx_t n)
{
    /* Ensure ω array is large enough for the potential search range */
    const size_t INITIAL_LIMIT = 2000000;   /* ample for n ≤ 10 000 */
    if (!omega_ready) init_omega_sieve(INITIAL_LIMIT);

    /* The core search */
    const uint8_t om_n = omega_table[n];
    idx_t k = 1;
    while (1) {
        /* Extend table when we would go out of bounds */
        if ((size_t)(n + k) > omega_limit)
            ensure_omega_limit((size_t)(n + k));

        const uint8_t om_k   = omega_table[k];
        const uint8_t om_nk  = omega_table[n + k];
        const uint8_t mx = om_n > om_k ? om_n : om_k;

        if (om_nk > mx)
            return (seq_t)k;

        ++k;
    }
}

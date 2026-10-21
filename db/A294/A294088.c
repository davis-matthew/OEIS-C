/*
 * A294088  Least prime p_k such that (p_k)^n has p_{k-1} as substring.
 * https://oeis.org/A294088
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   power_string_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294088: For n ≥ 2 return the smallest prime p_k such that the
   decimal representation of p_k^n contains the decimal representation of
   the preceding prime p_{k-1}.  For n > 59 the answer is known to be 3. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/power_string_uint32.h"

/* Precomputes primes up to 5000 (enough for all needed cases). */
static void init_primes(uint32_t *primes, size_t *cnt)
{
    const size_t LIMIT = 5000;
    bool is_prime[LIMIT + 1];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t k = p * p; k <= LIMIT; k += p)
                is_prime[k] = false;
        }
    }
    *cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[(*cnt)++] = (uint32_t)i;
}

/* Main sequence function as required by the framework. */
seq_t a(idx_t n)
{
    /* For n > 59 the answer is always 3. */
    if (n > 59) return (seq_t)3;

    /* Static storage for primes. */
    static bool initialized = false;
    static uint32_t primes[600];
    static size_t primes_cnt = 0;
    if (!initialized) {
        init_primes(primes, &primes_cnt);
        initialized = true;
    }

    char pow_buf[400];
    char prev_buf[20];

    /* Iterate over primes starting from the second prime (index 1). */
    for (size_t idx = 1; idx < primes_cnt; ++idx) {
        uint32_t p   = primes[idx];
        uint32_t prev = primes[idx - 1];

        power_string_uint32(p, n, pow_buf, sizeof(pow_buf));
        snprintf(prev_buf, sizeof(prev_buf), "%u", prev);

        if (strstr(pow_buf, prev_buf) != NULL)
            return (seq_t)p;
    }
    /* Should never reach here for n ≥ 2. */
    return (seq_t)0;
}

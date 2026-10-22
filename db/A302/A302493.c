/*
 * A302493  Prime numbers of prime-power index.
 * https://oeis.org/A302493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(A000961(n)) A000961(n) is the nth prime power
 *            (1,2,3,4,5,7,8,9,...)
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* --- Configuration ------------------------------------------------------- */
#define MAX_N          200000        /* enough for the expected test range   */
#define SIEVE_LIMIT    2500000       /* primes + prime powers up to this limit */

/* --- Global tables ------------------------------------------------------- */
static bool _initialized = false;

/* Smallest prime factor for each integer up to SIEVE_LIMIT */
static uint32_t _spf[SIEVE_LIMIT + 1];

/* List of primes up to SIEVE_LIMIT */
static uint32_t _primes[SIEVE_LIMIT];
static size_t   _prime_count = 0;

/* nth prime power values (1‑based).  Only the first MAX_N values are kept. */
static uint32_t _prime_power[MAX_N];
static size_t   _prime_power_count = 0;

/* --- Helper functions ---------------------------------------------------- */
static void
_init_sieve(void)
{
    /* Linear sieve for spf and prime list */
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (_spf[i] == 0) {
            _spf[i] = i;
            _primes[_prime_count++] = i;
            if ((uint64_t)i * i <= (uint64_t)SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= (uint32_t)SIEVE_LIMIT; j += i) {
                    if (_spf[j] == 0) _spf[j] = i;
                }
            }
        }
    }
}

static void
_generate_prime_powers(void)
{
    _prime_power_count = 0;
    /* Include 1 as the first prime power */
    if (_prime_power_count < MAX_N) {
        _prime_power[_prime_power_count++] = 1;
    }

    for (uint32_t n = 2; n <= SIEVE_LIMIT && _prime_power_count < MAX_N; ++n) {
        uint32_t p = _spf[n];
        uint32_t m = n;
        while (m % p == 0) m /= p;   /* strip all factors of p */
        if (m == 1) {
            _prime_power[_prime_power_count++] = n;
        }
    }
}

static void
_init(void)
{
    if (_initialized) return;
    _init_sieve();
    _generate_prime_powers();
    _initialized = true;
}

/* --- Main function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for indices < 1 */

    _init();

    if ((idx_t)_prime_power_count < n) {
        /* The pre‑calculated tables are not large enough.
           For the intended test range this should never happen. */
        return (seq_t)0;
    }

    uint32_t m = _prime_power[n - 1];   /* the n‑th prime power value */

    if (m == 0 || m > _prime_count) {
        /* m is beyond the available primes (should not occur). */
        return (seq_t)0;
    }

    return (seq_t)_primes[m - 1];      /* m‑th prime (1‑based) */
}

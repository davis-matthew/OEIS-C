/*
 * A394256  Exponentially odd numbers whose maximum exponent in their prime
 *          factorization is congruent to 1 mod 4.
 * https://oeis.org/A394256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394256: Exponentially odd numbers whose maximum exponent in
 * their prime factorization is congruent to 1 (mod 4).  The function runs a
 * simple search: each integer is tested for the property by trial division
 * with a pre‑computed prime table. */

/* --- Standard includes --------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* --- Compile‑time prime limit --------------------------------------------- */
#define PRIME_LIMIT 100000

/* --- Prime table and one‑time initialisation ----------------------------- */
static uint64_t PRIMES[PRIME_LIMIT];
static size_t PRIME_COUNT = 0;
static bool PRIMES_INITIALIZED = false;

static void init_primes(void)
{
    if (PRIMES_INITIALIZED)
        return;

    bool composite[PRIME_LIMIT + 1];
    memset(composite, 0, sizeof(composite));

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            PRIMES[PRIME_COUNT++] = i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    composite[j] = 1;
            }
        }
    }
    PRIMES_INITIALIZED = true;
}

/* --- Check whether a value is exponentially odd with max exponent ≡ 1 mod 4 */
static bool is_exponentially_odd(seq_t val)
{
    uint64_t m = (uint64_t)val;        /* value to be factorised (fits in 64 bits) */
    uint64_t tmp = m;
    uint64_t max_exp = 0;

    for (size_t k = 0; k < PRIME_COUNT && PRIMES[k] * PRIMES[k] <= tmp; ++k) {
        uint64_t p = PRIMES[k];
        if (tmp % p != 0)
            continue;
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if ((exp & 1U) == 0)          /* even exponent → reject */
            return false;
        if (exp > max_exp)
            max_exp = exp;
    }

    if (tmp > 1) {                     /* remaining prime factor with exponent 1 */
        if (1 > max_exp)
            max_exp = 1;
    }

    return (max_exp % 4U) == 1U;
}

/* --- Main sequence access function --------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();

    idx_t count = 0;
    uint64_t candidate = 2;            /* iterate over 2,3,4,… (uint64_t safe) */

    while (count < n) {
        seq_t cv = (seq_t)candidate;
        if (is_exponentially_odd(cv)) {
            ++count;
            if (count == n)
                return cv;
        }
        ++candidate;
    }

    /* Unreachable: the loop always terminates when n ≥ 1 */
    return (seq_t)candidate;
}

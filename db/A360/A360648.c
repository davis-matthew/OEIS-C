/*
 * A360648  Fully multiplicative with a(A027697(k)) = A027699(k) and
 *          a(A027699(k)) = A027697(k) for any k > 0.
 * https://oeis.org/A360648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, mult
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360648: fully multiplicative permutation swapping odious and evil primes. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Helper: compute integer exponentiation by squaring for seq_t values.   */
static seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1ULL)
            res *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return res;
}

/* --------------------------------------------------------------------- */
/* Prime tables and mapping.  Limit chosen to cover all primes that can
   appear as factors of n for n up to about 10 000, which is the range
   of interest for the tests. */
#define PRIME_LIMIT 200000

static bool initialized = false;
static size_t num_primes = 0;
static uint64_t *primes = NULL;

static size_t num_odious = 0;
static size_t num_evil = 0;
static uint64_t *odious_primes = NULL;
static uint64_t *evil_primes = NULL;

/* For primes ≤ PRIME_LIMIT, MAP[p] gives the partner prime.  0 means
   no partner (occurs only for primes whose rank exceeds the other set).*/
static uint64_t *MAP = NULL;

/* --------------------------------------------------------------------- */
/* Initialise prime tables and the odious/evil mapping. */
static void init(void)
{
    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) return;               /* unlikely but defensive */

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
    {
        if (!sieve[i]) continue;
        for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
            sieve[j] = false;
    }

    /* Count primes and allocate arrays. */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) ++num_primes;

    primes        = (uint64_t *)malloc(num_primes * sizeof(uint64_t));
    odious_primes = (uint64_t *)malloc(num_primes * sizeof(uint64_t));
    evil_primes   = (uint64_t *)malloc(num_primes * sizeof(uint64_t));

    size_t pi = 0, oi = 0, ei = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (!sieve[i]) continue;
        primes[pi++] = i;
        /* Classify by parity of 1‑bits in binary (odious/evil). */
        if ((__builtin_popcountll(i) & 1ULL) != 0ULL)
            odious_primes[oi++] = i;
        else
            evil_primes[ei++] = i;
    }
    num_odious = oi;
    num_evil   = ei;

    /* Mapping array. */
    MAP = (uint64_t *)calloc(PRIME_LIMIT + 1, sizeof(uint64_t));
    size_t pairs = (num_odious < num_evil) ? num_odious : num_evil;
    for (size_t i = 0; i < pairs; ++i)
    {
        MAP[odious_primes[i]] = evil_primes[i];
        MAP[evil_primes[i]]   = odious_primes[i];
    }

    free(sieve);
    initialized = true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1ULL) return 1;

    if (!initialized)
        init();

    /* Factor n. */
    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;

    for (size_t i = 0; i < num_primes && primes[i] * primes[i] <= tmp; ++i)
    {
        uint64_t p = primes[i];
        if (tmp % p != 0) continue;

        /* Count exponent. */
        uint64_t exp = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++exp;
        }

        /* Map prime if a partner exists. */
        uint64_t mapped = (p <= PRIME_LIMIT && MAP[p] != 0ULL) ? MAP[p] : p;
        result *= pow_seq((seq_t)mapped, exp);
    }

    if (tmp > 1ULL)   /* Remaining prime factor. */
    {
        uint64_t mapped = (tmp <= PRIME_LIMIT && MAP[tmp] != 0ULL) ? MAP[tmp] : tmp;
        result *= (seq_t)mapped;
    }

    return result;
}

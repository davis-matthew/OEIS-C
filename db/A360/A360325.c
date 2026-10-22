/*
 * A360325  a(n) is the largest divisor of n that has only prime-indexed prime
 *          factors.
 * https://oeis.org/A360325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = largest divisor of n whose all prime factors are
 *                prime-indexed primes (primes whose position among all primes
 *                is itself prime).  More formally:
 *   a(n) = ∏_{p | n, p∈S} p^{v_p(n)}
 *   where S = {p prime | index(p) is prime}.
 *
 * The implementation precomputes primes up to a fixed limit, stores
 * the list of primes and a Boolean array telling whether the
 * (1‑based) index of a prime is itself prime.  For a given n the
 * prime factors are obtained by trial division using this list.
 * The result is accumulated in seq_t and returned.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*  Largest number to pre‑compute primes for.
 *  A value of 2 000 000 is sufficient for n up to ~10 000 and still
 *  quick to sieve.  Increase if larger n may appear. */
#define PRIME_LIMIT 2000000U

/* Static data for the prime tables */
static bool          *g_prime_flag   = NULL;   /* is prime?  index 0..PRIME_LIMIT */
static uint32_t      *g_primes       = NULL;   /* list of primes */
static size_t        g_prime_cnt    = 0;        /* number of primes */
static bool          *g_prime_idx_is = NULL;   /* is the (1‑based) index prime? */
static bool          g_init_done    = false;

/* ------------------------------------------------------------------ */
/* Helper: initialize prime tables once                              */
static void
init_prime_tables(void)
{
    if (g_init_done)
        return;

    /* Allocate flag array for primality up to PRIME_LIMIT */
    g_prime_flag = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        g_prime_flag[i] = true;
    g_prime_flag[0] = g_prime_flag[1] = false;

    /* Simple sieve of Eratosthenes */
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (g_prime_flag[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                g_prime_flag[j] = false;
        }
    }

    /* Count primes and allocate array */
    g_prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (g_prime_flag[i])
            ++g_prime_cnt;

    g_primes = (uint32_t *)malloc(g_prime_cnt * sizeof(uint32_t));
    size_t pos = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (g_prime_flag[i])
            g_primes[pos++] = (uint32_t)i;

    /* Determine which prime indices are prime.
     * Indexes start at 1 for the first prime (2). */
    g_prime_idx_is = (bool *)malloc((g_prime_cnt + 1) * sizeof(bool));
    g_prime_idx_is[0] = false; /* unused */
    for (size_t i = 1; i <= g_prime_cnt; ++i) {
        /* Index i is prime iff g_prime_flag[i] is true */
        if (i <= PRIME_LIMIT)
            g_prime_idx_is[i] = g_prime_flag[i];
        else
            g_prime_idx_is[i] = false;   /* should not happen */
    }

    g_init_done = true;
}

/* ------------------------------------------------------------------ */
/* Binary search for a prime value in the pre‑computed list.
 * Returns the 0‑based index of the prime, or (size_t)-1 if not found.
 */
static size_t
find_prime_index(uint64_t p)
{
    size_t left = 0, right = g_prime_cnt;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if ((uint64_t)g_primes[mid] < p)
            left = mid + 1;
        else
            right = mid;
    }
    if (left < g_prime_cnt && (uint64_t)g_primes[left] == p)
        return left;
    return (size_t)-1;
}

/* ------------------------------------------------------------------ */
/* Main function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)       /* safety for offset < 1 */ return (seq_t)0;

    init_prime_tables();

    /* Factor n and accumulate only prime-indexed prime factors */
    idx_t temp = n;
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint32_t p = g_primes[i];
        uint64_t pp = (uint64_t)p;
        if (pp * pp > temp)
            break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* Include p^exp if its (1‑based) index is prime */
            if (g_prime_idx_is[i + 1]) {
                seq_t prime_seq = (seq_t)p;
                for (int k = 0; k < exp; ++k)
                    result *= prime_seq;
            }
        }
    }

    /* Remaining part greater than 1 is a prime factor itself */
    if (temp > 1) {
        uint64_t p = temp;
        size_t idx = find_prime_index(p);
        if (idx != (size_t)-1) {
            if (g_prime_idx_is[idx + 1]) {
                seq_t prime_seq = (seq_t)p;  /* exponent = 1 */
                result *= prime_seq;
            }
        }
    }

    return result;
}

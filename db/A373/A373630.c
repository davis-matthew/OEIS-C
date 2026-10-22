/*
 * A373630  a(n) is the n-th semiprime after the n-th prime.
 * https://oeis.org/A373630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 4681, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373630
   Formula: a(n) = the n-th semiprime after the n-th prime
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const idx_t LIMIT = 2000000;   /* enough for n <= 10000 */

static bool          initialized = false;
static idx_t        *primes      = NULL;
static size_t        prime_count = 0;
static uint32_t     *spf         = NULL;

/* build the smallest prime factor sieve and the list of primes */
static void ensure_initialized(void)
{
    if (initialized)
        return;

    /* smallest prime factor for every number up to LIMIT */
    spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= LIMIT / i) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }

    /* list of primes (we allocate more space than needed) */
    primes = (idx_t *)malloc((LIMIT / 10 + 1000) * sizeof(idx_t));
    prime_count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            primes[prime_count++] = i;
        }
    }

    initialized = true;
}

/* a(n) = the n-th semiprime after the n-th prime */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* below OEIS offset */

    ensure_initialized();

    if (n > prime_count)
        return 0;          /* safety check: not enough primes precomputed */

    idx_t p = primes[n - 1];  /* n-th prime (1‑based) */

    /* search for the n-th semiprime after p */
    idx_t    num  = p + 1;
    uint32_t count = 0;

    for (;; ++num) {
        if (num > LIMIT) {
            /* For the supported range this should never happen */
            abort();
        }

        /* test whether num is a semiprime using spf */
        idx_t tmp = num;
        int    factorcnt = 0;
        while (tmp > 1 && factorcnt <= 2) {
            tmp /= spf[tmp];
            ++factorcnt;
        }
        if (factorcnt == 2) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}

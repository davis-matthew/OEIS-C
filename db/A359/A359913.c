/*
 * A359913  Numbers whose multiset of prime factors has integer median.
 * https://oeis.org/A359913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer >1 whose multiset of prime factors has
 *            an * integer median. It's defined by searching successive
 *            integers and keeping * only those where that condition holds
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static data for sieve and caching */
static uint32_t *spf   = NULL;   /* smallest prime factor array   */
static uint32_t *primes = NULL;  /* list of primes                */
static size_t   prime_cnt = 0;    /* number of primes              */
static seq_t  *cache     = NULL;  /* memoised terms of the sequence */
static size_t  cache_size  = 0;    /* number of cached terms         */
static size_t  cache_cap   = 0;    /* allocated elements             */
static uint32_t current = 2;       /* next integer to test           */
static bool    initialized = false;

/* ---------- helper ----------------------------------------------------- */

/* Build a linear sieve up to LIMIT.  The arrays are allocated at runtime. */
static void init_sieve(unsigned LIMIT)
{
    /* Allocate spf array */
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    memset(spf, 0, (LIMIT + 1) * sizeof(uint32_t));

    /* Allocate primes array with maximum possible size.  This is safe enough
     * for LIMIT up to a few million. */
    primes = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));

    size_t pc = 0;                          /* count of primes found   */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                  /* i is prime              */
            spf[i] = i;
            primes[pc++] = i;
        }
        /* Linear sieve: stop when prime exceeds spf[i] or product out of range. */
        for (size_t j = 0; j < pc && primes[j] <= spf[i]
                && (uint64_t)i * primes[j] <= LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    prime_cnt = pc;
}

/* Return true if the integer X (>1) has an integer median of its prime factors. */
static bool has_integer_median(uint32_t X)
{
    /* Extract prime factors in non‑decreasing order by successive division
     * using the spf array.  The number of factors will never exceed 32 for
     * X up to the limits we use here. */
    uint32_t factors[32];
    size_t len = 0;
    while (X > 1) {
        uint32_t p = spf[X];
        factors[len++] = p;
        X /= p;
    }

    if (len == 0)
        return false;           /* X == 1 never occurs in the sequence */

    if (len & 1)                /* odd number of factors → median is an element */
        return true;

    /* even number of factors: median = (factors[len/2-1] + factors[len/2]) / 2
     * which is an integer iff the sum is even.  Equivalently, the two middle
     * primes have the same parity (both odd or both equal to 2). */
    uint32_t a = factors[len / 2 - 1];
    uint32_t b = factors[len / 2];
    return ((a + b) & 1) == 0; /* sum even → integer median */
}

/* ---------- public interface ------------------------------------------- */

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1.  For n < 1, behaviour is irrelevant. */
    if (n < 1)
        return 0;

    /* Initialize data structures on the first call.  The LIMIT is chosen
     * to be comfortably larger than the value of a(10000) (approximately 12500). */
    if (!initialized) {
        const unsigned LIMIT = 2000000;   /* 2 million is ample for the required n */
        init_sieve(LIMIT);
        initialized = true;
    }

    /* Grow the cache until we have the requested term.  Each new integer is
     * tested for the integer‑median property and, if it passes, appended to
     * the cache.  The test is independent of previous entries, so the order
     * of calls does not matter. */
    while (cache_size < (size_t)n) {
        /* Find the next candidate with the property. */
        while (true) {
            if (has_integer_median(current)) {
                /* Cache the term. */
                if (cache_size >= cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)current;
                break;
            }
            ++current;
        }
        ++current;   /* move on to the next integer for the next loop */
    }

    return cache[n - 1];
}

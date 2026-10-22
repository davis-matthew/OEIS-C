/*
 * A366134  Number of primes between prime(n) and prime(n)+2*log(prime(n)),
 *          exclusive.
 * https://oeis.org/A366134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{primes p such that prime(n) < p < prime(n) +
 *            2*log(prime(n))}
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (exclusive interval). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*   Prime sieve:  supports primes up to about 2,000,000             */
/* ------------------------------------------------------------------ */

static uint64_t *g_primes   = NULL;     /* array of primes */
static size_t    g_prime_count = 0;      /* number of primes stored */

static void init_prime_sieve(void)
{
    if (g_primes != NULL)
        return;          /* already initialized */

    /* ---------------------------------------------------------------- */
    /*  Choose a sieve limit.  prime(10000) = 104729; 2*log(104729) ≈ 23.2.
       So prime(10000)+2*log ≈ 104752. A limit of 2,000,000 is far more
       than sufficient and inexpensive to compute.                       */
    /* ---------------------------------------------------------------- */
    const size_t LIMIT = 2000000;

    /* Allocate a composite flag array.  bool occupies one byte.        */
    bool *is_composite = (bool *)malloc(LIMIT + 1);
    if (!is_composite)
        return;          /* allocation failed – we fall back to zero answers */

    for (size_t i = 2; i <= LIMIT; ++i)
        is_composite[i] = false;

    /* Simple sieve of Eratosthenes. */
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_composite[j] = true;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i])
            ++cnt;

    g_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!g_primes) {
        free(is_composite);
        return;          /* allocation failed */
    }

    /* Fill prime array. */
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i])
            g_primes[idx++] = (uint64_t)i;

    g_prime_count = cnt;
    free(is_composite);
}

/* ------------------------------------------------------------------ */
/*   Helper: first index > value (upper bound) on a sorted array       */
/* ------------------------------------------------------------------ */
static size_t upper_bound_uint64(const uint64_t *arr,
                                 size_t          len,
                                 uint64_t        val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;          /* number of elements <= val */
}

/* ------------------------------------------------------------------ */
/*   OEIS sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* behaviour below offset irrelevant */

    init_prime_sieve();

    if (!g_primes || (size_t)n > g_prime_count)
        return (seq_t)0;          /* out of pre‑computed range */

    /* prime(n) is the (n-1)th element (0‑based). */
    uint64_t p = g_primes[(size_t)n - 1];

    /* Compute the upper boundary: p + 2*log(p).  Use double to get
       fractional part, then floor to an integer upper bound.        */
    double bound = (double)p + 2.0 * log((double)p);
    /* Guard against rounding errors: add a tiny epsilon.               */
    uint64_t intbound = (uint64_t)floor(bound + 1e-12);

    /* Count primes <= intbound  minus  primes <= p. */
    size_t count_le_intbound = upper_bound_uint64(g_primes, g_prime_count, intbound);
    size_t count_le_p        = upper_bound_uint64(g_primes, g_prime_count, p);

    /* Number of primes strictly between p and intbound. */
    if (count_le_intbound > count_le_p)
        return (seq_t)(count_le_intbound - count_le_p);
    else
        return (seq_t)0;
}

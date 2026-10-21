/*
 * A248164  Table read by rows: n-th row contains the products of all
 *          consecutive subsets of the first n primes in their natural order.
 * https://oeis.org/A248164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) = list of products of all consecutive prime subsets, read
 *            by rows. The n-th term is obtained by determining the row k such
 *            that the first k(k+1)(k+2)/6 terms cover index n, then finding
 *            the appropriate length and starting position of the consecutive
 *            subset
 * Range:     u8 n <= 31, u16 n <= 80, u32 n <= 219, u64 n <= 815,
 *            u128 n <= 2925, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Allocate a static list of primes up to a fixed limit.
   20000 is more than enough to cover the first 140 primes (required for n≈10000). */
static uint64_t *g_primes = NULL;
static size_t g_prime_cnt = 0;

static void init_primes(void)
{
    if (g_primes != NULL)
        return;                       /* already initialized */

    const uint64_t LIMIT = 20000;      /* safe upper bound for the needed primes */
    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_composite)
        return;                       /* allocation failure – sequence will be incomplete */

    g_prime_cnt = 0;
    g_primes = (uint64_t *)malloc(LIMIT * sizeof(uint64_t));
    if (!g_primes) {
        free(is_composite);
        return;
    }

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            /* store prime */
            g_primes[g_prime_cnt++] = i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }

    free(is_composite);
}

/* Helper to compute k such that the cumulative count of rows up to k
   (k*(k+1)*(k+2)/6) is >= idx, by binary search using 128‑bit arithmetic. */
static uint64_t row_index(idx_t idx)
{
    uint64_t low = 0, high = 1;
    /* Find an upper bound for high */
    while ((__uint128_t)high * (high + 1) * (high + 2) / 6 < idx)
        high <<= 1;

    /* Binary search for smallest high with cumulative >= idx */
    while (low + 1 < high) {
        uint64_t mid = (low + high) >> 1;
        __uint128_t cum = (__uint128_t)mid * (mid + 1) * (mid + 2) / 6;
        if (cum >= idx)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* defined only for n >= 1 */

    /* Ensure primes are ready */
    init_primes();
    if (g_primes == NULL)
        return 0;               /* failed to initialise primes */

    uint64_t N = (uint64_t)n;

    /* 1. Find the row k that contains the n-th term */
    uint64_t k = row_index(N);

    /* 2. Compute cumulative number of terms before row k */
    __uint128_t prev_cum = (__uint128_t)(k - 1) * k * (k + 1) / 6;
    uint64_t p = N - (uint64_t)prev_cum;   /* 1‑based position inside row k */

    /* 3. Find the length of the consecutive subset */
    uint64_t len = 1;
    uint64_t cum = 0;
    while (1) {
        uint64_t count = k - len + 1;      /* how many subsets of this length */
        if (cum + count >= p)
            break;
        cum += count;
        ++len;
    }
    uint64_t start = p - cum;              /* 1‑based start index */

    /* 4. Compute the product of the primes from start to start+len-1 */
    seq_t prod = 1;
    for (uint64_t i = start - 1; i < start - 1 + len; ++i)
        prod *= (seq_t)g_primes[i];

    return prod;
}

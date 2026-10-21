/*
 * A254763  One half of the fundamental positive solution y = y2(n) of the
 *          second class of the Pell equation x^2 - 2*y^2 = A007519(n), n>=1
 *          (primes congruent to 1 mod 8).
 * https://oeis.org/A254763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2) * y2, where y2 is the second smallest positive y
 *            for which x² – 2*y² = p, with p = nth prime ≡ 1 (mod 8). The
 *            algorithm: 1. Generate a list of primes ≡ 1 (mod 8) large enough
 *            to contain the requested index using a simple sieve. 2. For the
 *            given n, take p = primes1mod8[n-1]. 3. Search...
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */
extern seq_t a(idx_t n);

/* ---- 1. prime sieve for primes ≡ 1 (mod 8) --------------------------- */

static uint64_t *primes1mod8 = NULL;   /* array of primes ≡1 (mod 8)       */
static size_t primes1_cnt = 0;         /* number of primes stored          */
static size_t primes1_cap = 0;         /* allocated capacity of the array   */

static void init_primes1mod8(void)
{
    if (primes1mod8 != NULL)        /* already initialized */
        return;

    /* Upper bound that safely contains at least 10 000 primes ≡1 (mod 8);
       4 000 000 has ~70 000 such primes.                                    */
    const uint64_t LIMIT = 4000000ULL;
    /* Allocate sieve array.  Use a char (1 byte) per entry.                */
    char *is_prime = (char *)calloc((size_t)LIMIT + 1, sizeof(char));
    if (is_prime == NULL) abort();

    /* Eratosthenes sieve.  All entries initially marked as prime.         */
    for (uint64_t i = 2; i <= LIMIT; ++i)
        is_prime[i] = 1;

    is_prime[0] = 0; is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count how many primes ≡1 (mod 8) to preallocate the array.          */
    size_t count = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i] && (i & 7ULL) == 1ULL)
            ++count;
    }

    primes1_cap = count;
    primes1mod8 = (uint64_t *)malloc(primes1_cap * sizeof(uint64_t));
    if (primes1mod8 == NULL) abort();

    /* Fill the array.                                                     */
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i] && (i & 7ULL) == 1ULL) {
            primes1mod8[idx++] = i;
        }
    }
    primes1_cnt = idx;

    free(is_prime);
}

/* ---- 2. integer perfect‑square test --------------------------------- */
static bool is_square_uint64(uint64_t v, uint64_t *root)
{
    uint64_t low = 0, high = (uint64_t)1ULL << 32; /* since (2^32)^2 > 2^64-1   */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq == v) {
            *root = mid;
            return true;
        }
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return false;
}

/* ---- 3. main sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset 1; undefined behaviour below 1 */
        return 0;

    init_primes1mod8();

    if ((size_t)n > primes1_cnt)
        abort();     /* The predefined LIMIT should cover requested n   */

    uint64_t p = primes1mod8[(size_t)n - 1];   /* nth prime ≡1 (mod 8)  */

    /* Search for the first two values of y with p + 2*y^2 a perfect square. */
    uint64_t y1 = 0, y2 = 0;
    bool first_found = false;
    for (uint64_t y = 1; ; ++y) {
        __int128 tmp = (__int128)2 * (__int128)y * (__int128)y + (__int128)p;
        if (tmp > (__int128)UINT64_MAX)   /* cannot happen for n ≤ 10000   */
            break;

        uint64_t val = (uint64_t)tmp;
        uint64_t root;
        if (is_square_uint64(val, &root)) {
            if (!first_found) {
                first_found = true;
                y1 = y;
            } else {
                y2 = y;
                break;
            }
        }
    }

    if (y2 == 0)      /* safety check – in theory never happens      */
        abort();

    /* y2 is always even for primes ≡1 (mod 8).  Return y2/2 as seq_t.     */
    return (seq_t)(y2 / 2);
}

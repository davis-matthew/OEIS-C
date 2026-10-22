/*
 * A365573  Number of primes between prime(n) and prime(n)+log(prime(n)),
 *          exclusive.
 * https://oeis.org/A365573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes between prime(n) and
 *            prime(n)+⌊log(prime(n))⌋, exclusive. (log is natural logarithm)
 * Range:     u8 n <= 53, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* size_t */

static struct {
    seq_t *data;
    size_t size;
    size_t cap;
} prime_cache = {NULL, 0, 0};

/* Grow the prime cache if needed */
static void ensure_prime_cap(size_t needed)
{
    if (prime_cache.cap < needed) {
        size_t new_cap = prime_cache.cap ? prime_cache.cap * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_data = (seq_t *)realloc(prime_cache.data,
                                           new_cap * sizeof(seq_t));
        prime_cache.data = new_data;
        prime_cache.cap = new_cap;
    }
}

/* Append a prime to the cache */
static void add_prime(uint64_t cand)
{
    ensure_prime_cap(prime_cache.size + 1);
    prime_cache.data[prime_cache.size++] = (seq_t)cand;
}

/* Generate the next prime after the last in the cache */
static void generate_next_prime(void)
{
    uint64_t cand;
    if (prime_cache.size == 0) {
        cand = 2;
    } else {
        cand = (uint64_t)prime_cache.data[prime_cache.size - 1] + 1;
        if (cand % 2 == 0) cand += 1;   /* skip even numbers > 2 */
    }

    /* Trial division by existing primes */
    for (;;) {
        bool prime = true;
        uint64_t cand_test = cand;
        for (size_t i = 0; i < prime_cache.size; ++i) {
            uint64_t p = (uint64_t)prime_cache.data[i];
            if (p * p > cand_test) break;
            if (cand_test % p == 0) { prime = false; break; }
        }
        if (prime) break;
        cand += 2;  /* next odd candidate */
    }
    add_prime(cand);
}

/* Precomputed thresholds: smallest integer > e^k, for k = 0,1,...  */
static const uint64_t LOG_THRESH[] = {
    1ULL, 3ULL, 8ULL, 21ULL, 55ULL, 149ULL, 404ULL, 1097ULL, 2981ULL,
    8104ULL, 22027ULL, 59875ULL, 162755ULL, 442414ULL, 1202605ULL,
    3269018ULL, 8886111ULL, 24154953ULL, 65659970ULL, 178482303ULL,
    485165196ULL, 1322124606ULL, 3591123637ULL
};
static const size_t LOG_THRESH_SZ =
    sizeof(LOG_THRESH) / sizeof(LOG_THRESH[0]);

/* Compute floor(log(p)) for p ≤ 2^64 using the threshold table */
static unsigned int floor_log(uint64_t p)
{
    unsigned int k = 0;
    while (k + 1 < LOG_THRESH_SZ && LOG_THRESH[k + 1] <= p) ++k;
    return k;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1; undefined for 0 */

    /* Ensure we have at least n primes */
    while (prime_cache.size < (size_t)n) generate_next_prime();

    seq_t nth_prime = prime_cache.data[n - 1];
    uint64_t p64 = (uint64_t)nth_prime;
    unsigned int lk = floor_log(p64);
    uint64_t limit64 = p64 + lk;   /* upper bound integer */

    /* Ensure primes up to limit are generated */
    while (prime_cache.size == 0 ||
           (uint64_t)prime_cache.data[prime_cache.size - 1] <= limit64)
        generate_next_prime();

    /* Binary search for first prime > limit */
    size_t lo = 0, hi = prime_cache.size;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if ((uint64_t)prime_cache.data[mid] <= limit64)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t idx_upper = lo;                 /* #primes ≤ limit */
    seq_t count = (seq_t)(idx_upper - (size_t)n);  /* exclude prime(n) itself */

    return count;
}

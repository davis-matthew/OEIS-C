/*
 * A352827  Heinz numbers of integer partitions y with a fixed point y(i) = i.
 *          Such a fixed point is unique if it exists.
 * https://oeis.org/A352827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Heinz number of a partition with a fixed point
 * This implementation enumerates integers starting from 2,
 * factors each into primes, builds the corresponding partition
 * (prime indices repeated by multiplicity), sorts it
 * in descending order and checks if some part equals its position.
 * The first such numbers are cached and returned in order.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ----------------------------------------------------------------- */
/* Prime sieve limited to a fixed bound (enough for the required range) */
#define PRIME_SIEVE_LIMIT 100000
static uint64_t primes[PRIME_SIEVE_LIMIT];
static size_t prime_count = 0;

/* Build prime list once */
static void build_primes(void)
{
    if (prime_count != 0) return;
    bool *sieve = (bool *)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    if (!sieve) return;                 /* allocation failure is ignored */
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            primes[prime_count++] = i;
            if (i * i <= PRIME_SIEVE_LIMIT) {
                for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
}

/* Binary search for prime index. Returns 1‑based index. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid + 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;   /* never reached for valid primes */
}

/* ----------------------------------------------------------------- */
/* Sequence cache (enough for n up to 200 000, way above the limit) */
#define MAX_CACHE 200000
static seq_t seq_cache[MAX_CACHE];
static size_t seq_len = 0;       /* number of stored terms */
static uint64_t next_m = 2;      /* next integer to test */

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out‑of‑bounds, not expected */

    build_primes();

    while (seq_len < (size_t)n) {
        uint64_t m = next_m;
        uint64_t x = m;
        size_t parts[64];
        size_t plen = 0;

        /* factor m using precomputed primes */
        for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= x; ++i) {
            uint64_t p = primes[i];
            while (x % p == 0) {
                if (plen < 64) parts[plen++] = i + 1;   /* index is 1‑based */
                x /= p;
            }
        }
        if (x > 1) {   /* remaining prime factor larger than sieve limit */
            size_t idx = prime_index(x);
            if (idx) {
                if (plen < 64) parts[plen++] = idx;
            }
        }

        /* sort parts descending (insertion sort, small array) */
        for (size_t i = 1; i < plen; ++i) {
            size_t j = i;
            while (j > 0 && parts[j - 1] < parts[j]) {
                size_t tmp = parts[j];
                parts[j] = parts[j - 1];
                parts[j - 1] = tmp;
                --j;
            }
        }

        /* check for a fixed point */
        bool has_fixed = false;
        for (size_t i = 0; i < plen; ++i) {
            if (parts[i] == i + 1) { has_fixed = true; break; }
        }

        if (has_fixed && seq_len < MAX_CACHE) {
            seq_cache[seq_len++] = (seq_t)m;
        }

        ++next_m;
    }

    return seq_cache[n - 1];
}

/*
 * A342695  a(n) is the number of primes in an n X n square array that do not
 *          appear on its border, with the elements of the square array being
 *          the numbers from 1..n^2, listed in increasing order by rows.
 * https://oeis.org/A342695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n*(n-1)) - pi(n) - Sum_{k=1..n-2} (pi(n*k+1) -
 *            pi(n*k))
 * Range:     u8 n <= 42, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A342695 using a sieve of Eratosthenes
   for primality testing and a binary search for prime counting.
   The implementation is thread‑safe as it uses static data that
   grows only when a larger limit is required. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* ----------------- Static data ----------------- */
static uint8_t *prime_bits = NULL;   /* bitset for odd composite flags */
static size_t bitset_bytes = 0;

static uint32_t *prime_list = NULL;  /* array of primes */
static size_t prime_count = 0;
static size_t prime_cap = 0;

static uint64_t sieve_limit = 0;

/* ----------------- Bitset helpers ----------------- */
static inline bool test_composite(uint64_t x)
{
    /* assumes x is odd and > 2 */
    uint64_t idx = (x - 3u) >> 1u;      /* index of odd x */
    return (prime_bits[idx >> 3] >> (idx & 7)) & 1u;
}

static inline void set_composite(uint64_t x)
{
    uint64_t idx = (x - 3u) >> 1u;
    prime_bits[idx >> 3] |= (uint8_t)(1u << (idx & 7));
}

/* ----------------- Sieve builder ----------------- */
static void build_sieve(uint64_t limit)
{
    /* free old structures */
    if (prime_bits) free(prime_bits);
    if (prime_list) free(prime_list);

    /* size of bitset for odd numbers only */
    size_t odd_count = 0;
    if (limit >= 3u)
        odd_count = ((limit - 3u) >> 1u) + 1u;
    bitset_bytes = (odd_count + 7u) >> 3u;
    prime_bits = (uint8_t*)calloc(bitset_bytes, 1u);
    if (!prime_bits) abort();   /* allocation failure */

    uint64_t root = (uint64_t)sqrt((double)limit);
    for (uint64_t i = 3u; i <= root; i += 2u) {
        if (!test_composite(i)) {
            uint64_t step = i << 1u;              /* 2*i */
            uint64_t start = i * i;
            for (uint64_t j = start; j <= limit; j += step)
                set_composite(j);
        }
    }

    /* build prime list */
    prime_cap = (limit / 10u) + 100u;   /* rough estimate - oversize */
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) abort();

    prime_count = 0;
    if (limit >= 2u) {
        prime_list[prime_count++] = 2u;
    }
    for (uint64_t n = 3u; n <= limit; n += 2u) {
        if (!test_composite(n)) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1u;
                prime_list = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = (uint32_t)n;
        }
    }

    sieve_limit = limit;
}

/* Ensure sieve covers limit; build if necessary */
static void ensure_sieve(uint64_t limit)
{
    if (limit > sieve_limit)
        build_sieve(limit);
}

/* ----------------- Prime counting via binary search ----------------- */
static size_t count_primes_le(uint32_t m)
{
    /* number of primes <= m */
    size_t low = 0, high = prime_count;
    while (low < high) {
        size_t mid = low + ((high - low) >> 1);
        if (prime_list[mid] <= m)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/* ----------------- Prime test for numbers <= sieve_limit ----------------- */
static inline bool is_prime_uint64(uint64_t x)
{
    if (x < 2u) return false;
    if (x == 2u) return true;
    if ((x & 1u) == 0u) return false;
    return !test_composite(x);
}

/* ----------------- Main sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n < 1u) return 0;   /* undefined below offset, but safe */

    uint64_t limit = (uint64_t)n * (n - 1u);   /* upper bound m */
    ensure_sieve(limit);

    /* pi(m) and pi(n) */
    seq_t pi_m = (seq_t)count_primes_le((uint32_t)limit);
    seq_t pi_n = (seq_t)count_primes_le((uint32_t)n);

    /* count of primes of form n*k+1 for 1 <= k <= n-2 */
    seq_t count_form = 0u;
    if (n >= 3u) {
        for (uint64_t k = 1u; k <= n - 2u; ++k) {
            uint64_t p = n * k + 1u;
            if (is_prime_uint64(p))
                ++count_form;
        }
    }

    return pi_m - pi_n - count_form;
}

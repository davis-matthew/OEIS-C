/*
 * A376305  Run-compression of the sequence of first differences of squarefree
 *          numbers.
 * https://oeis.org/A376305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the run‑compressed list of first differences of
 *            squarefree numbers
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic cache for the compressed terms */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;   /* allocated capacity */
static size_t a_size  = 0;   /* number of terms already stored */

/* State for generating squarefree numbers on demand */
static uint64_t next_candidate = 1;   /* next integer to test for squarefree */
static uint64_t last_sq        = 0;   /* last found squarefree number */
static bool have_last_sq       = false;

/* Small sieve of primes up to 1000 for squarefree testing */
#define PRIME_LIMIT 1000
static uint32_t primes[168];   /* number of primes <= 1000 is 168 */
static size_t primes_cnt = 0;
static bool primes_init = false;

static void init_primes(void)
{
    if (primes_init) return;
    bool composite[PRIME_LIMIT + 1] = {false};
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!composite[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                composite[j] = true;

    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!composite[i]) primes[primes_cnt++] = (uint32_t)i;
    primes_init = true;
}

/* Check whether an integer is squarefree. */
static bool is_squarefree(uint64_t n)
{
    init_primes();
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t p2 = p * p;
        if (p2 > n) break;
        if (n % p2 == 0) return false;
    }
    return true;
}

/* Ensure a_cache has at least 'min_cap' capacity. */
static void ensure_cache_capacity(size_t min_cap)
{
    if (a_cap >= min_cap) return;
    size_t new_cap = a_cap == 0 ? 16 : a_cap;
    while (new_cap < min_cap) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Main interface: compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /* The sequence is 1-indexed; protect against n == 0. */
    if (n == 0) return 0;

    while (a_size < (size_t)n) {
        /* Find next squarefree number. */
        while (true) {
            if (is_squarefree(next_candidate)) {
                uint64_t cur_sq = next_candidate;
                ++next_candidate;

                if (!have_last_sq) {
                    /* First squarefree number seen. */
                    last_sq      = cur_sq;
                    have_last_sq = true;
                } else {
                    /* Compute difference and apply run compression. */
                    seq_t diff = (seq_t)(cur_sq - last_sq);
                    if (a_size == 0 || diff != a_cache[a_size - 1]) {
                        ensure_cache_capacity(a_size + 1);
                        a_cache[a_size++] = diff;
                    }
                    last_sq = cur_sq;
                }
                break; /* proceed to next term */
            }
            ++next_candidate;
        }
    }

    /* Return the requested term. */
    return a_cache[n - 1];
}

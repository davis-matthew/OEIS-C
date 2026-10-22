/*
 * A380413  Terms appearing twice in A378086 (number of nonsquarefree numbers
 *          < prime(n)).
 * https://oeis.org/A380413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   cast_malloc, cast_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380413: terms that appear twice in A378086.
   In terms of A378086 (number of nonsquarefree integers < prime(k)):
   a(n) is that count for prime(k) and for the next prime(k+1), i.e.
   a(n) = A378086(A068361(n)) = A378086(A068361(n)+1).  */

/* Note: For an arbitrary‑precision build, seq_t is a BigInt class
   and idx_t is uint64_t, so all allocations must cast the result from
   malloc/realloc.  For the 64‑bit build, seq_t is an unsigned integer
   type that can hold all terms.  The code below works for either
   configuration. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/cast_malloc.h"
#include "../utils/cast_realloc.h"

/* Forward declaration required by the build system */
seq_t a(idx_t n);

/* --------------------------------------------------------------------
   Parameters – choose a limit that gives > 20000 repeated values
   (enough for n up to 10000).  The 100kth prime is about 1.3 M; 3 M
   comfortably covers that.  Adjust if needed.  ------------------------------------------------------------------ */
#define SIEVE_LIMIT 3000000U
#define INITIAL_PRIME_CAP 200000U
#define INITIAL_REPEATS_CAP 20000U

/* --------------------------------------------------------------------
   State – static variables shared by all calls.
   ------------------------------------------------------------------ */
static unsigned char *is_prime      = NULL;   /* sieve marker */
static unsigned char *is_non_sq    = NULL;   /* nonsquarefree marker */
static uint32_t *prime_list        = NULL;   /* list of primes */
static size_t prime_count          = 0;      /* number of primes found */

static seq_t *repeat_cache        = NULL;   /* recorded repeated values */
static size_t repeat_count        = 0;      /* number of repeats stored */

/* --------------------------------------------------------------------
   Build a prime sieve and mark all nonsquarefree numbers < SIEVE_LIMIT.
   ------------------------------------------------------------------ */
static void build_sieve_and_non_squarefree(void)
{
    is_prime   = (unsigned char*)cast_malloc(SIEVE_LIMIT + 1);
    is_non_sq  = (unsigned char*)cast_malloc(SIEVE_LIMIT + 1);
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) {
        is_prime[i] = 1;
        is_non_sq[i] = 0;
    }
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t p = 2; (uint64_t)p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint32_t j = p * p; j <= SIEVE_LIMIT; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    /* prime list */
    prime_list = (uint32_t*)cast_malloc(INITIAL_PRIME_CAP * sizeof(uint32_t));
    prime_count = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            if (prime_count == INITIAL_PRIME_CAP) {
                size_t new_cap = prime_count ? prime_count * 2 : INITIAL_PRIME_CAP;
                prime_list = (uint32_t*)cast_realloc(prime_list,
                                                     new_cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = i;
        }
    }

    /* nonsquarefree numbers: multiples of p^2 */
    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint32_t p = prime_list[idx];
        uint64_t p2 = (uint64_t)p * p;
        if (p2 > SIEVE_LIMIT) break;
        for (uint64_t j = p2; j <= SIEVE_LIMIT; j += p2) {
            is_non_sq[j] = 1;
        }
    }
}

/* --------------------------------------------------------------------
   Compute the repeated values and store them in repeat_cache.
   ------------------------------------------------------------------ */
static void compute_repeats(void)
{
    repeat_cache = (seq_t*)cast_malloc(INITIAL_REPEATS_CAP * sizeof(seq_t));
    repeat_count = 0;
    /* A378086 for the prime immediately before the first prime (2) is 0. */
    seq_t prev_count = 0;
    seq_t prev_value = 0;
    bool first = true;
    uint32_t prev_prime = 2U;   /* first prime */

    /* To count nonsquarefree numbers in each prime interval,
       scan the numbers once and keep a running sum.  */
    seq_t run_sum = 0;
    size_t idx_num = 0;            /* index into is_non_sq, 0..SIEVE_LIMIT */

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];

        /* Count nonsquarefree < p, i.e. from previous prime to p-1 */
        while (idx_num < (size_t)(p - 1)) {
            ++idx_num; /* move to next number */
            if (is_non_sq[idx_num]) ++run_sum;
        }

        /* run_sum now equals A378086 for this prime p. */
        seq_t curr_value = run_sum;

        if (!first && curr_value == prev_value) {
            if (repeat_count == (size_t)(INITIAL_REPEATS_CAP * 2)) {
                size_t new_cap = repeat_count * 2;
                repeat_cache = (seq_t*)cast_realloc(repeat_cache,
                                                     new_cap * sizeof(seq_t));
            } else if (repeat_count == INITIAL_REPEATS_CAP) {
                size_t new_cap = repeat_count * 2;
                repeat_cache = (seq_t*)cast_realloc(repeat_cache,
                                                     new_cap * sizeof(seq_t));
            }
            repeat_cache[repeat_count++] = curr_value;
        }

        prev_value = curr_value;
        first = false;
    }
}

/* --------------------------------------------------------------------
   Public function – return a(n).  Builds tables on first call.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        build_sieve_and_non_squarefree();
        compute_repeats();
        initialized = true;
    }

    if (n < 1) return (seq_t)0;
    if ((size_t)n > repeat_count) return (seq_t)0;
    return repeat_cache[(size_t)n - 1];
}

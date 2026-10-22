/*
 * A379054  a(n) = composite(2*n+2) - composite(2*n).
 * https://oeis.org/A379054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = composite(2*n+2) - composite(2*n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Auxiliary: initial prime sieve up to a fixed limit.
   We store both a boolean array of primes and a list of primes for trial division. */
static const size_t PRIME_LIMIT = 50000;          /* enough for n ≤ 10 000 */
static bool *is_prime_bool = NULL;                /* true if i is prime   */
static uint32_t *prime_list = NULL;              /* list of primes       */
static size_t prime_count = 0;                    /* count of primes      */

/* Helper: array of composite numbers (0‑based index).
   composite_vals[0] = 4, composite_vals[1] = 6, … */
#define COMPOSITE_CAP 40000                       /* > required 2*n+2 for n ≤ 10 000 */
static seq_t composite_vals[COMPOSITE_CAP];
static size_t composite_cnt = 0;                  /* number of composites stored */
static uint64_t last_candidate = 3;               /* next number to test (4) */

static void init_sieve(void)
{
    if (is_prime_bool != NULL) return;           /* already initialized */

    is_prime_bool = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime_bool) exit(1);                 /* allocation failure */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime_bool[i] = true;
    is_prime_bool[0] = is_prime_bool[1] = false;

    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (is_prime_bool[p]) {
            for (size_t k = p * p; k <= PRIME_LIMIT; k += p)
                is_prime_bool[k] = false;
        }
    }

    /* build list of primes */
    prime_list = (uint32_t*)calloc(PRIME_LIMIT + 1, sizeof(uint32_t));
    if (!prime_list) exit(1);
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime_bool[i]) prime_list[prime_count++] = (uint32_t)i;
}

/* Extend composite_vals until it contains at least 'need' entries. */
static void ensure_composites_upto(size_t need)
{
    for (; composite_cnt < need && composite_cnt < COMPOSITE_CAP; ++last_candidate) {
        bool isprime = false;
        if (last_candidate <= PRIME_LIMIT) {
            isprime = is_prime_bool[last_candidate];
        } else {
            /* trial division for numbers beyond the pre‑computed limit */
            isprime = true;
            for (size_t i = 0; i < prime_count; ++i) {
                uint32_t p = prime_list[i];
                if ((uint64_t)p * p > last_candidate) break;
                if (last_candidate % p == 0) { isprime = false; break; }
            }
        }
        if (!isprime) composite_vals[composite_cnt++] = (seq_t)last_candidate;
    }
    /* In the unlikely case 'need' > COMPOSITE_CAP, we silently stop – the problem
       constraints guarantee this will not happen for n ≤ 10 000. */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                          /* out of range – undefined */

    init_sieve();

    /* Since a(n) = composite(2*n+2) - composite(2*n),
       we need the (2*n+2)-th composite.  Composite indices start at 1. */
    size_t need = (size_t)(((__int128)n) * 2 + 2);
    ensure_composites_upto(need);

    size_t idx1 = (size_t)(((__int128)n) * 2 - 1); /* 2*n   -> zero‑based index */
    size_t idx2 = (size_t)(((__int128)n) * 2 + 1); /* 2*n+2 -> zero‑based index */

    seq_t diff = composite_vals[idx2] - composite_vals[idx1];
    return diff;
}

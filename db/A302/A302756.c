/*
 * A302756  a(n) is the least possible greatest prime in any partition of
 *          prime(n) into three primes; n >= 4.
 * https://oeis.org/A302756
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302756: minimal greatest prime r in a partition
   prime(n) = p + q + r with p <= q <= r, n >= 4
   (search/computation based on the definition) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

static uint32_t *prime_list = NULL;   /* primes up to current limit */
static size_t prime_count = 0;        /* number of primes stored */
static idx_t prime_limit = 0;         /* highest number that has been sieved */

/* Build a list of all primes <= limit using a simple sieve.
   The primes are stored in prime_list and prime_count is updated.
   prime_limit is set to the requested limit. */
static void build_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit) return; /* already built up to this limit */

    /* Allocate a temporary sieve array (0 = prime, 1 = composite) */
    char *sieve = (char *)calloc((size_t)(limit + 1), sizeof(char));
    if (!sieve) return; /* out of memory */

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* Count the primes to allocate the primes array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++cnt;

    /* Allocate or reallocate the primes array */
    uint32_t *new_list = (uint32_t *)realloc(prime_list, cnt * sizeof(uint32_t));
    if (!new_list) { free(sieve); return; } /* out of memory */
    prime_list = new_list;
    prime_count = cnt;

    /* Fill the primes array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) prime_list[idx++] = (uint32_t)i;
    }

    free(sieve);
    prime_limit = limit;
}

/* Return the minimal greatest prime r in a partition
   of prime(n) into three primes p <= q <= r. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;   /* offset behavior is not important */

    /* Get the actual prime value (p).  get_prime returns a 32‑bit prime. */
    uint32_t p_val = get_prime(n);

    /* Build the prime table up to p_val to enable efficient checks. */
    build_primes_up_to((idx_t)p_val);

    /* Search for the minimal r starting from the smallest prime. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t r_val = prime_list[i];            /* current candidate r */
        if (r_val > p_val) break;                  /* cannot exceed p */

        uint32_t S_val = p_val - r_val;            /* remaining sum for p + q */
        if (S_val < 4) continue;                   /* need two primes >= 2 */

        /* The largest possible p is min(r, S/2).  No need to test larger. */
        uint32_t max_p = (r_val < (S_val / 2)) ? r_val : (S_val / 2);

        /* Find the last index of a prime <= max_p. */
        size_t j_max = 0;
        while ((j_max + 1) < prime_count &&
               prime_list[j_max + 1] <= max_p) ++j_max;

        /* Try all p values up to j_max. */
        for (size_t j = 0; j <= j_max; ++j) {
            uint32_t p_val_i = prime_list[j];
            uint32_t q_val = S_val - p_val_i;
            if (q_val < p_val_i) continue;             /* enforce p <= q */
            if (q_val > r_val) continue;               /* enforce q <= r */

            /* Test primality of q.  q_val fits in 32 bits, so is_prime_small works. */
            if (is_prime_small((seq_t)q_val)) {
                return (seq_t)r_val;                       /* minimal r found */
            }
        }
    }

    /* For all valid primes (n >= 4) a valid partition exists by the
       Goldbach weak conjecture, so this return should never be reached. */
    return 0;
}

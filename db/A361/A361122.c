/*
 * A361122  0 if n-th composite number A002808(n) is divisible by 3, otherwise
 *          1.
 * https://oeis.org/A361122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:
   a(n) = 0 if the n‑th composite number A002808(n) is divisible by 3,
          1 otherwise.
   The sequence starts with n=1 corresponding to 4.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static storage for primes and composites */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0, primes_cap = 0;

static uint64_t *comps  = NULL;
static size_t   comps_cnt  = 0, comps_cap = 0;

/* next integer to examine (>=4) */
static uint64_t next_check = 4;

/* ensures the storage arrays are allocated */
static void init_storage(void) {
    if (!primes) {
        primes_cap = 64;
        primes = (uint64_t *)realloc(NULL, primes_cap * sizeof(uint64_t));
        if (!primes) exit(EXIT_FAILURE);
        primes[0] = 2;
        primes[1] = 3;
        primes_cnt = 2;
    }
    if (!comps) {
        comps_cap = 64;
        comps = (uint64_t *)realloc(NULL, comps_cap * sizeof(uint64_t));
        if (!comps) exit(EXIT_FAILURE);
        comps_cnt = 0;
    }
}

/* ensures array has capacity for at least one more element */
static void grow_array(uint64_t **arr, size_t *cnt, size_t *cap) {
    if (*cnt >= *cap) {
        *cap = (*cap == 0) ? 64 : (*cap << 1);    /* double the cap */
        *arr = (uint64_t *)realloc(*arr, (*cap) * sizeof(uint64_t));
        if (!*arr) exit(EXIT_FAILURE);
    }
}

/* test primality using the primes built so far */
static bool is_prime(uint64_t m) {
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > m) break;
        if (m % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined offset guard */

    init_storage();

    while (comps_cnt < (size_t)n) {
        uint64_t m = next_check++;
        if (is_prime(m)) {
            grow_array(&primes, &primes_cnt, &primes_cap);
            primes[primes_cnt++] = m;
        } else {
            grow_array(&comps, &comps_cnt, &comps_cap);
            comps[comps_cnt++] = m;
        }
    }

    uint64_t comp = comps[n - 1];
    return (comp % 3ULL == 0) ? (seq_t)0 : (seq_t)1;
}

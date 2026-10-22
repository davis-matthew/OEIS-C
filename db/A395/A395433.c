/*
 * A395433  a(n) is the median of the odd primes less than 2n.
 * https://oeis.org/A395433
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 296, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Median of odd primes less than 2n (OEIS A395433) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the primes and sieve */
static idx_t max_sieved = 0;
static bool *is_composite = NULL;
static size_t prime_count = 0;
static uint64_t *prime_list = NULL;

/* extend the sieve to cover all primes <= limit */
static void extend_sieve(idx_t limit) {
    if (limit <= max_sieved) return;

    /* free previous data */
    if (is_composite) free(is_composite);
    is_composite = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_composite) return;           /* out of memory – abort silently */
    max_sieved = limit;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_composite[j] = true;
            }
        }
    }

    /* collect primes */
    if (prime_list) free(prime_list);
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) ++cnt;
    }
    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) prime_list[idx++] = (uint64_t)i;
    }
    prime_count = cnt;
}

/* compute a(n) */
seq_t a(idx_t n) {
    if (n < 2) return 0;                     /* undefined for n < 2 */

    idx_t upper = 2 * n - 1;                 /* primes < 2n */
    extend_sieve(upper);

    /* number of odd primes < 2n = total primes - 1 (to skip 2) */
    size_t odd_cnt = prime_count - 1;
    if (odd_cnt == 0) return 0;              /* should not happen for n>=2 */

    uint64_t median;
    if (odd_cnt & 1) {                       /* odd count – single median */
        size_t pos = 1 + odd_cnt / 2;        /* skip the first prime (2) */
        median = prime_list[pos];
    } else {                                 /* even count – average of two middles */
        size_t low  = 1 + (odd_cnt / 2 - 1);
        size_t high = 1 + odd_cnt / 2;
        median = (prime_list[low] + prime_list[high]) / 2;
    }
    return (seq_t)median;
}

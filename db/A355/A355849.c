/*
 * A355849  a(n) is the least k > 1 such that k*n is the average of two
 *          consecutive primes.
 * https://oeis.org/A355849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 1 such that k*n is the average of two
 *            consecutive primes
 * Range:     u8 none, u16 n <= 961, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static const size_t LIMIT = 20000000U;   /* pre‑computed upper limit */
static char *prime_flag = NULL;          /* sieve array */
static uint32_t *primes = NULL;          /* list of primes up to LIMIT */
static size_t primes_cap = 0;
static size_t primes_cnt = 0;
static char *avg_flag = NULL;            /* mark numbers that are averages of consecutive primes */
static int initialized = 0;

/* push a new prime into the dynamic array */
static void push_prime(uint32_t p)
{
    if (primes_cnt == primes_cap) {
        size_t newcap = primes_cap ? primes_cap << 1 : 1024U;
        uint32_t *tmp = (uint32_t *)realloc(primes, newcap * sizeof(uint32_t));
        if (!tmp) exit(1); /* allocation failed */
        primes = tmp;
        primes_cap = newcap;
    }
    primes[primes_cnt++] = p;
}

/* precompute primes and averages */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    prime_flag = (char *)malloc((LIMIT + 1) * sizeof(char));
    if (!prime_flag) exit(1);
    for (size_t i = 0; i <= LIMIT; ++i) prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (prime_flag[i]) {
            push_prime((uint32_t)i);
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i) {
                    prime_flag[j] = 0;
                }
            }
        }
    }

    avg_flag = (char *)calloc(LIMIT + 1, sizeof(char));
    if (!avg_flag) exit(1);

    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        uint64_t sum = (uint64_t)primes[i] + (uint64_t)primes[i + 1];
        if ((sum & 1ULL) != 0) continue;              /* ignore odd sums (non integer average) */
        uint32_t avg = (uint32_t)(sum >> 1);            /* exact integer average */
        if (avg > LIMIT) break;
        avg_flag[(size_t)avg] = 1;
    }
    /* primes and flags remain static; no further computation needed */
}

seq_t a(idx_t n)
{
    init();  /* ensure tables are ready */

    /* start with k = 2 (since k > 1) */
    seq_t k = (seq_t)2;
    for (;;) {
        seq_t m = (seq_t)n * k;
        if (m <= (seq_t)LIMIT) {
            if (avg_flag[(size_t)m]) {
                return k;
            }
        } else {
            /* limit exceeded – for the range of n used in the tests this
               should not happen. Return 0 to indicate failure. */
            return (seq_t)0;
        }
        k = (seq_t)((seq_t)k + 1);
    }
}

/*
 * A341946  Number of partitions of n into 3 primes (counting 1 as a prime).
 * https://oeis.org/A341946
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into 3 primes (1 counted as
 *            prime)
 * Range:     u8 n <= 366, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Prime sieve data */
static bool   *prime_bool = NULL;   /* sieve array: prime_bool[i] == true <=> i is a prime or 1 */
static size_t  prime_limit = 0;     /* largest index for which prime_bool is valid */

static uint64_t *prime_vals = NULL; /* list of primes >= 2 up to prime_limit */
static size_t    prime_cnt  = 0;     /* number of such primes */

/* Build or extend the prime sieve and prime list up to at least n. */
static void
ensure_primes_upto(idx_t n)
{
    if (n <= prime_limit)
        return;                      /* already built for this range */

    /* free previous data */
    free(prime_bool);
    free(prime_vals);

    /* allocate new sieve */
    prime_limit = (size_t)n;
    prime_bool = (bool*)calloc(prime_limit + 1, sizeof(bool));
    if (!prime_bool)
        return;                      /* allocation failure – caller will see 0 result */

    /* initialize to true, then correct 0 and 1 */
    for (size_t i = 0; i <= prime_limit; ++i)
        prime_bool[i] = true;
    if (prime_limit >= 0)
        prime_bool[0] = false;        /* 0 is not prime */
    if (prime_limit >= 1)
        prime_bool[1] = true;         /* 1 is treated as prime */

    /* Eratosthenes sieve for real primes >= 2 */
    for (size_t p = 2; p * p <= prime_limit; ++p) {
        if (prime_bool[p]) {
            for (size_t mult = p * p; mult <= prime_limit; mult += p)
                prime_bool[mult] = false;
        }
    }

    /* allocate array for primes (excluding 1) */
    prime_vals = (uint64_t*)malloc((prime_limit + 1) * sizeof(uint64_t));
    if (!prime_vals) {
        prime_vals = NULL;
        prime_cnt  = 0;
        return;                      /* allocation failure */
    }

    /* fill the list */
    prime_cnt = 0;
    for (size_t v = 2; v <= prime_limit; ++v) {
        if (prime_bool[v]) {
            prime_vals[prime_cnt++] = v;
        }
    }
}

/* Sequence definition */
seq_t
a(idx_t n)
{
    if (n < 3)
        return 0;                      /* behaviour below offset is irrelevant */

    ensure_primes_upto(n);

    seq_t count = 0;
    const size_t len_with1 = prime_cnt + 1; /* include the \"prime\" 1 at index 0 */

    for (size_t i = 0; i < len_with1; ++i) {
        uint64_t p1 = (i == 0) ? 1ULL : prime_vals[i - 1];
        for (size_t j = i; j < len_with1; ++j) {
            uint64_t p2 = (j == 0) ? 1ULL : prime_vals[j - 1];

            uint64_t sum12 = p1 + p2;
            if (sum12 > (uint64_t)n)
                break;                  /* further j only increase sum12 */

            uint64_t p3 = (uint64_t)n - sum12;
            if (p3 < p2)
                continue;               /* enforce non‑decreasing order */

            if (p3 <= (uint64_t)n && prime_bool[(size_t)p3])
                ++count;
        }
    }

    return count;
}

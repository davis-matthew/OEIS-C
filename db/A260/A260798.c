/*
 * A260798  Number of partitions of p=prime(n) into aliquant parts (i.e.,
 *          parts that do not divide p, meaning any part except 1 and p).
 * https://oeis.org/A260798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(prime(n)) - p(prime(n)-1) - 1 where p(m) is the
 *            partition function
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 36, u64 n <= 90, u128 n <= 246,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of partitions of the _n-th prime into parts
   that are neither 1 nor the prime itself.   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static tables for primes and partition numbers. */
static uint64_t *prime_array = NULL;   /* primes in order */
static size_t prime_count  = 0;        /* number of primes stored */
static size_t prime_cap    = 0;        /* allocated capacity of prime_array */
static uint64_t next_candidate = 2;    /* next integer to test for primality */

static seq_t *partition = NULL;        /* partition table */
static size_t part_max   = 0;          /* largest n with partition[n] ready */
static size_t part_cap   = 0;          /* allocated capacity of partition */

/* Generate the next prime and append it to prime_array. */
static void generate_next_prime(void)
{
    for (;;) {
        uint64_t cand = next_candidate++;
        bool isprime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_array[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime) {
            if (prime_count >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 10;
                uint64_t *tmp = (uint64_t *)realloc(prime_array, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_array = tmp;
                prime_cap   = new_cap;
            }
            prime_array[prime_count++] = cand;
            return;
        }
    }
}

/* Ensure that prime_array contains at least the first n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;
    while (prime_count < (size_t)n) generate_next_prime();
}

/* Expand the partition table up to index up_to (inclusive). */
static void ensure_partitions(idx_t up_to)
{
    if (part_max >= (size_t)up_to) return;

    if (part_cap == 0 || part_cap <= (size_t)up_to) {
        size_t new_cap = part_cap ? part_cap : 10;
        while (new_cap <= (size_t)up_to) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(partition, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        partition = tmp;
        part_cap   = new_cap;
    }

    /* Base case for the partition function. */
    partition[0] = 1;

    for (idx_t n = part_max + 1; n <= up_to; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            seq_t term = partition[n - g1];
            if (k & 1) sum += term; else sum -= term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            term = partition[n - g2];
            if (k & 1) sum += term; else sum -= term;
        }
        partition[n] = sum;
    }
    part_max = up_to;
}

/* ----------  Public function required by the test harness ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* out of range, though tests start at 1 */

    /* Get the nth prime. */
    ensure_primes(n);
    uint64_t p = prime_array[n - 1];

    /* Compute partition numbers up to p. */
    ensure_partitions(p);

    /* a(p) = P(p) - P(p-1) - 1 */
    seq_t res = partition[p];
    res -= partition[p - 1];
    if (res > 0) res -= 1;      /* remove the trivial partition [p] */
    return res;
}

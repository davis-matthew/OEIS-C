/*
 * A305937  Number of partitions such that the least positive integer which is
 *          not a part of the partition is prime.
 * https://oeis.org/A305937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p prime, p(p-1)/2 ≤ n} ( p( n - p(p-1)/2 ) - p( n -
 *            p(p-1)/2 - p ) ), with p(k)=0 for k<0, a(0)=0
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 133, u64 n <= 427,
 *            u128 n <= 1483, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for partition numbers */
static seq_t *part = NULL;      /* p(n) array */
static idx_t part_max = 0;      /* largest n computed */

/* Static storage for primes */
static uint64_t *primes = NULL; /* list of primes */
static size_t prime_cnt = 0;    /* number of primes stored */
static size_t prime_cap = 0;    /* primes generated up to prime_cap */

/* Ensure that primes up to 'limit' are available */
static void ensure_primes(idx_t limit) {
    if (limit <= prime_cap) return;
    free(primes);
    primes = NULL;
    prime_cnt = 0;
    prime_cap = 0;

    size_t sz = (size_t)limit + 1;
    bool *sieve = (bool*)malloc(sz);
    if (!sieve) return;
    for (size_t i = 0; i < sz; ++i) sieve[i] = true;
    if (sz > 0) sieve[0] = false;
    if (sz > 1) sieve[1] = false;

    for (size_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    /* Count primes */
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++prime_cnt;

    primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) { free(sieve); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) primes[idx++] = (uint64_t)i;

    prime_cap = limit;
    free(sieve);
}

/* Ensure partition numbers up to 'up' are computed */
static void ensure_partitions(idx_t up) {
    if (up <= part_max) return;

    /* Allocate or grow the partition array */
    if (!part) {
        part = (seq_t*)malloc((up + 1) * sizeof(seq_t));
        if (!part) return;
        part[0] = 1;      /* p(0) = 1 */
        part_max = 0;
    } else {
        seq_t *newp = (seq_t*)realloc(part, (up + 1) * sizeof(seq_t));
        if (!newp) return;
        part = newp;
    }

    for (idx_t n = part_max + 1; n <= up; ++n) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            seq_t term = part[n - g1];
            if (k & 1) pos += term; else neg += term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= n) {
                term = part[n - g2];
                if (k & 1) pos += term; else neg += term;
            }
        }
        part[n] = pos - neg;
    }
    part_max = up;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0U) return 0;          /* a(0) = 0 */

    /* Compute partitions up to n */
    ensure_partitions(n);

    /* Find largest prime needed: p(p-1)/2 ≤ n */
    idx_t max_p = 1;
    for (idx_t p = 2; ; ++p) {
        idx_t base = (p * (p - 1)) / 2;
        if (base > n) break;
        max_p = p;
    }

    /* Generate primes up to max_p */
    ensure_primes(max_p);

    seq_t total = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > max_p) break;
        idx_t base = (p * (p - 1)) / 2;
        idx_t nprime = n - base;
        seq_t term = part[nprime];
        if (nprime >= p) {
            term -= part[nprime - p];
        }
        total += term;
    }
    return total;
}

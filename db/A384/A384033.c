/*
 * A384033  a(n) is the number of solutions to n = sopfr(k*sopfr(n)) where
 *          sopfr(m) is sum of prime factors of m counted with multiplicity.
 * https://oeis.org/A384033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000607(n - sopfr(sopfr(n))) if sopfr(sopfr(n)) <= n,
 *            otherwise 0. Here A000607(m) is the number of partitions of m
 *            into prime parts
 * Range:     u8 n <= 48, u16 n <= 118, u32 n <= 363, u64 n <= 1342,
 *            u128 n <= 5455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- global data ---------------------------------------------------------- */
static idx_t prime_limit = 0;        /* largest number up to which primes are known   */
static uint32_t *prime_list = NULL;  /* array of primes                              */
static size_t prime_cnt   = 0;       /* number of primes in prime_list               */

static seq_t *part = NULL;          /* DP array for prime partitions                */
static idx_t part_limit = 0;        /* highest index computed in part                */

/* --- primes ---------------------------------------------------------------- */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* simple sieve up to 'limit' (limit is at most ~1e5 for n≈1e4) */
    size_t sz = limit + 1;
    bool *is_prime = (bool*)calloc(sz, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    uint32_t *new_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_list) exit(EXIT_FAILURE);
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) new_list[k++] = (uint32_t)i;

    free(prime_list);
    prime_list = new_list;
    prime_cnt  = cnt;
    prime_limit = limit;
    free(is_prime);
}

/* --- sum of prime factors with multiplicity -------------------------------- */
static seq_t sopfr_idx(idx_t n)
{
    seq_t sum = 0;
    idx_t temp = n;
    if (temp <= 1) return sum;

    ensure_primes((idx_t)2);  /* at least true to start factorization */

    for (size_t pi = 0; pi < prime_cnt && (uint64_t)prime_list[pi] * prime_list[pi] <= temp; ++pi) {
        uint32_t p = prime_list[pi];
        while (temp % p == 0) {
            sum += (seq_t)p;
            temp /= p;
        }
    }
    if (temp > 1) sum += (seq_t)temp;   /* remaining prime factor */
    return sum;
}

/* --- prime partitions ------------------------------------------------------ */
static void ensure_partitions(idx_t limit)
{
    if (limit <= part_limit) return;

    /* resize array */
    part = (seq_t*)realloc(part, (limit + 1) * sizeof(seq_t));
    if (!part) exit(EXIT_FAILURE);

    /* initialize positions above old limit to zero */
    for (idx_t i = part_limit + 1; i <= limit; ++i) part[i] = 0;
    part[0] = 1;  /* empty partition is one way */

    /* compute partitions using primes up to 'limit' */
    ensure_primes(limit);

    /* reset for full recomputation to guarantee correctness */
    for (idx_t i = 0; i <= limit; ++i) part[i] = (seq_t)0;
    part[0] = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = prime_list[pi];
        if (p > limit) break;
        for (idx_t sum = p; sum <= limit; ++sum) {
            part[sum] += part[sum - p];
        }
    }

    part_limit = limit;
}

/* --- sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* guard; offset is 1, but safe for n=0 */

    seq_t s1 = sopfr_idx(n);
    seq_t s2 = sopfr_idx((idx_t)s1);

    if (s2 > n) return 0;

    idx_t m = n - (idx_t)s2;

    ensure_partitions(m);
    return part[m];
}

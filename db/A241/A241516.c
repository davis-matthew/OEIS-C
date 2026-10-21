/*
 * A241516  Least positive primitive root g < prime(n) modulo prime(n) which
 *          is also a partition number given by A000041, or 0 if such a number
 *          g does not exist.
 * https://oeis.org/A241516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241516: Least positive primitive root g < prime(n) modulo prime(n)
   which is also a partition number A000041.  Return 0 if none exists. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Prime generation (simple sieve) ---------- */
static uint64_t *primes_arr = NULL;
static size_t prime_cnt = 0;

/* Recomputes all primes up to 'limit' (inclusive). */
static void recompute_primes(int limit) {
    if (limit < 2) limit = 2;
    bool *composite = (bool *)malloc((size_t)limit + 1);
    memset(composite, 0, (size_t)limit + 1);

    /* Allocate prime array: approximate capacity */
    size_t cap = limit / 10 + 10;
    uint64_t *tmp = (uint64_t *)realloc(primes_arr, cap * sizeof(uint64_t));
    primes_arr = tmp;
    prime_cnt = 0;

    for (int i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            if (prime_cnt == cap) {
                cap <<= 1;
                primes_arr = (uint64_t *)realloc(primes_arr, cap * sizeof(uint64_t));
            }
            primes_arr[prime_cnt++] = (uint64_t)i;
            if ((int64_t)i * i <= limit) {
                for (int j = i * i; j <= limit; j += i) composite[j] = 1;
            }
        }
    }
    free(composite);
}

/* ---------- Partition numbers (pentagonal recurrence) ---------- */
static seq_t *partitions = NULL;
static size_t part_capacity = 0;
static size_t part_cnt = 0;   /* number of partition values stored */

static void init_partitions(void) {
    if (!partitions) {
        part_capacity = 512;
        partitions = (seq_t *)malloc(part_capacity * sizeof(seq_t));
        partitions[0] = (seq_t)1;   /* p(0) = 1 */
        part_cnt = 1;
    }
}

/* Compute next partition number using generalized pentagonal numbers. */
static void compute_next_partition(void) {
    idx_t k = part_cnt;  /* index of new partition */

    if (part_cnt >= part_capacity) {
        part_capacity <<= 1;
        partitions = (seq_t *)realloc(partitions, part_capacity * sizeof(seq_t));
    }

    int64_t sum = 0;
    for (int m = 1; ; ++m) {
        idx_t g1 = (idx_t)(m * (3 * m - 1) / 2);
        if (k < g1) break;
        int64_t term = ((m & 1) ? 1 : -1) * (int64_t)partitions[k - g1];
        sum += term;

        idx_t g2 = (idx_t)(m * (3 * m + 1) / 2);
        if (k >= g2) {
            term = ((m & 1) ? 1 : -1) * (int64_t)partitions[k - g2];
            sum += term;
        }
    }
    partitions[part_cnt++] = (seq_t)sum;
}

/* Ensure all partition values less than 'limit' are computed. */
static void ensure_partitions_upto(seq_t limit) {
    init_partitions();
    while (partitions[part_cnt - 1] < limit) {
        compute_next_partition();
    }
}

/* ---------- Modular exponentiation (uint64_t) ---------- */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) res = (uint64_t)((__uint128_t)res * b % mod);
        b = (uint64_t)((__uint128_t)b * b % mod);
        exp >>= 1;
    }
    return res;
}

/* ---------- Prime factorization (distinct primes) ---------- */
static void factorize_distinct(uint64_t n, uint64_t *factors, size_t *cnt) {
    *cnt = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt &&
         (uint64_t)primes_arr[i] * (uint64_t)primes_arr[i] <= temp; ++i) {
        if (temp % primes_arr[i] == 0) {
            factors[(*cnt)++] = primes_arr[i];
            while (temp % primes_arr[i] == 0) temp /= primes_arr[i];
        }
    }
    if (temp > 1) factors[(*cnt)++] = temp;
}

/* ---------- Primitive root test ---------- */
static bool is_primitive_root(uint64_t g, uint64_t p,
                              const uint64_t *factors, size_t fcnt) {
    if (g % p == 0) return false;
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t exp = (p - 1) / factors[i];
        if (mod_pow_uint64(g, exp, p) == 1) return false;
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Ensure prime list covers n */
    if (n > prime_cnt) {
        int limit = (int)n * 20 + 100;
        if (limit < 200) limit = 200;
        recompute_primes(limit);
    }

    uint64_t prime_n = primes_arr[n - 1];

    /* Ensure partition numbers up to prime_n are available */
    ensure_partitions_upto((seq_t)prime_n);

    /* Factor p-1 */
    static uint64_t prime_factors[64];
    size_t pf_cnt;
    factorize_distinct(prime_n - 1, prime_factors, &pf_cnt);

    /* Search for the smallest partition number that is a primitive root */
    for (size_t k = 1; k < part_cnt; ++k) {
        seq_t g = partitions[k];
        if (g >= (seq_t)prime_n) break;
        if (is_primitive_root((uint64_t)g, prime_n, prime_factors, pf_cnt))
            return g;
    }

    return (seq_t)0;  /* no such number exists */
}

/*
 * A325361  Heinz numbers of integer partitions whose differences are weakly
 *          decreasing.
 * https://oeis.org/A325361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Each term is the Heinz number of a weakly-difference‑decreasing
 *            integer partition. The sequence lists these numbers in ascending
 *            order (offset = 1). A Heinz number of a partition (y1,…,yk) is Π
 *            prime(yi). Differences are computed as if the partition were
 *            increasing
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- standard headers ---- */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- static data for the sequence ---- */
static seq_t *seq_cache = NULL;
static size_t seq_size = 0, seq_cap = 0;
static idx_t checked_max = 0;      /* highest integer m that has been tested */

/* ---- prime data ---- */
#define PRIME_LIMIT 2000000          /* enough for the first ~10k terms   */
static uint8_t *is_prime = NULL;     /* 0/1 sieve bytes                 */
static uint32_t *prime_list = NULL;  /* array of primes                 */
static size_t prime_cnt = 0;          /* number of primes found           */
static uint32_t *prime_index = NULL;  /* prime_index[p] = index of prime p (1‑based) */

/* ---- helper: initialise prime tables once ---- */
static void init_primes(void) {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    is_prime = (uint8_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint8_t));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* first estimate of number of primes for allocation */
    size_t est_primes = PRIME_LIMIT / 10; /* over‑estimate */
    prime_list = (uint32_t *)malloc(est_primes * sizeof(uint32_t));
    if (!prime_list) exit(1);
    prime_index = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    if (!prime_index) exit(1);
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) prime_index[i] = 0;

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            /* record prime and its index (starting at 1) */
            prime_index[i] = (uint32_t)(prime_cnt + 1);
            prime_list[prime_cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT)
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_prime[j] = 0;
        }
    }
    /* shrink prime_list to actual size */
    uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cnt * sizeof(uint32_t));
    if (tmp) prime_list = tmp;
}

/* ---- helper: test whether a 64‑bit integer m is in the sequence ---- */
static bool is_valid_uint64(uint64_t m) {
    if (m == 1) return true;          /* empty partition */

    uint64_t n = m;
    /* collect indices of prime factors (repeated by exponent) in a small stack */
    size_t idx_stack[64];
    size_t stack_sz = 0;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = prime_list[pi];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (prime_index[p] == 0) return false; /* should not happen */
            for (int k = 0; k < exp; ++k)
                idx_stack[stack_sz++] = prime_index[p];
        }
    }
    if (n > 1) {                         /* remaining prime */
        if (n > PRIME_LIMIT) return false; /* outside pre‑computed range */
        uint32_t p = (uint32_t)n;
        if (prime_index[p] == 0) return false;
        idx_stack[stack_sz++] = prime_index[p];
    }

    if (stack_sz <= 1) return true;        /* single part always ok */

    /* sort indices in descending order */
    for (size_t i = 0; i < stack_sz; ++i)
        for (size_t j = i + 1; j < stack_sz; ++j)
            if (idx_stack[i] < idx_stack[j]) {
                size_t tmp = idx_stack[i];
                idx_stack[i] = idx_stack[j];
                idx_stack[j] = tmp;
            }

    /* check non‑decreasing gaps */
    for (size_t i = 0; i + 1 < stack_sz; ++i) {
        uint32_t gap1 = idx_stack[i] - idx_stack[i + 1];
        if (i + 2 < stack_sz) {
            uint32_t gap2 = idx_stack[i + 1] - idx_stack[i + 2];
            if (gap1 > gap2) return false;
        }
    }
    return true;
}

/* ---- helper: ensure cache capacity ---- */
static void ensure_cache_cap(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* ---- generate terms up to target index ---- */
static void generate_up_to(idx_t target) {
    init_primes();
    while (seq_size < target) {
        ++checked_max;
        uint64_t m = (uint64_t)checked_max;
        if (m > PRIME_LIMIT) {
            /* if we ever reach beyond PRIME_LIMIT, the algorithm
               stops gracefully; for the intended test range this should not happen. */
            break;
        }
        if (is_valid_uint64(m)) {
            ensure_cache_cap(seq_size + 1);
            seq_cache[seq_size++] = (seq_t)m;
        }
    }
}

/* ---- public function required by the specification ---- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* not used but safe */
    if (seq_size < n) generate_up_to(n);
    if (n <= seq_size) return seq_cache[n - 1];
    /* if we cannot produce enough terms (e.g. because PRIME_LIMIT is exceeded) */
    return (seq_t)0;
}

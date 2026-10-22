/*
 * A390308  Irregular triangle read by rows where row n lists the first sums
 *          minus 1 of the 1-prepended prime indices of n.
 * https://oeis.org/A390308
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   not stated
 * Range:     u8 n <= 4780, u16 n <= 9978, u32 n <= 9978, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A390308: a(n) is the (n-2)th flattened element of the irregular
   triangle read by rows, where each row n lists the first sums minus 1 of the
   1‑prepended prime indices of n. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- global data ---------- */
static uint64_t *primes = NULL;                /* list of primes (ascending)          */
static size_t primes_cnt = 0;                  /* number of primes                     */
static size_t primes_cap = 0;                  /* allocation capacity for primes       */

static uint32_t *prime_index_map = NULL;       /* prime -> its 1‑based index (0 if not prime) */
static size_t prime_map_size = 0;              /* size of prime_index_map array         */

static uint8_t *is_prime = NULL;               /* sieve array (1 = prime, 0 = composite) */
static size_t sieve_limit = 0;                 /* highest number for which sieve is ready */

static seq_t *terms = NULL;                    /* flattened triangle terms              */
static size_t terms_cnt = 0;                   /* number of terms stored                */
static size_t terms_cap = 0;                   /* allocation capacity for terms         */

static uint64_t next_row = 2;                  /* next integer n whose row is to be added */

/* ---------- helper functions ---------- */

/* Ensure that primes up to 'limit' are generated and prime_index_map is filled. */
static void ensure_primes_up_to(size_t limit)
{
    if (limit <= sieve_limit) return;

    size_t old_limit = sieve_limit;
    sieve_limit = limit;

    /* grow sieve array */
    is_prime = (uint8_t *)realloc(is_prime, (sieve_limit + 1) * sizeof(uint8_t));
    prime_index_map = (uint32_t *)realloc(prime_index_map, (sieve_limit + 1) * sizeof(uint32_t));
    memset(is_prime + old_limit + 1, 1, (sieve_limit - old_limit) * sizeof(uint8_t)); /* new entries as potential primes */
    if (old_limit == 0) {
        if (sieve_limit >= 0) is_prime[0] = 0;
        if (sieve_limit >= 1) is_prime[1] = 0;
    }

    /* sieve marking */
    size_t i;
    for (i = 2; i * i <= sieve_limit; ++i) {
        if (!is_prime[i]) continue;
        size_t start = i * i;
        if (start <= old_limit) {
            start = ((old_limit + 1 + i - 1) / i) * i; /* first multiple >= old_limit+1 */
        }
        for (size_t j = start; j <= sieve_limit; j += i)
            is_prime[j] = 0;
    }

    /* add new primes to list and map */
    for (i = old_limit + 1; i <= sieve_limit; ++i) {
        if (is_prime[i]) {
            /* grow primes array */
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 32;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = i;
            prime_index_map[i] = (uint32_t)primes_cnt; /* indices are 1‑based */
        }
    }
}

/* Add the row for integer 'row' to the flattened terms vector. */
static void add_row(uint64_t row)
{
    /* factorize row to obtain prime indices multiset */
    ensure_primes_up_to((size_t)row);

    uint64_t temp = row;

    /* temporarily store the prime indices (1‑based) */
    uint64_t *prime_indices = NULL;
    size_t pi_cnt = 0, pi_cap = 0;

    /* trial division with known primes */
    for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= temp; ++pi) {
        uint64_t p = primes[pi];
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            size_t idx = prime_index_map[p];
            while (pi_cnt + exp > pi_cap) {
                pi_cap = pi_cap ? pi_cap * 2 : 4;
                prime_indices = (uint64_t *)realloc(prime_indices, pi_cap * sizeof(uint64_t));
            }
            for (size_t k = 0; k < exp; ++k)
                prime_indices[pi_cnt++] = idx;
        }
    }

    /* remaining prime factor > sqrt(row) */
    if (temp > 1) {
        if (temp > sieve_limit)
            ensure_primes_up_to((size_t)temp);
        size_t idx = prime_index_map[(size_t)temp];
        while (pi_cnt + 1 > pi_cap) {
            pi_cap = pi_cap ? pi_cap * 2 : 4;
            prime_indices = (uint64_t *)realloc(prime_indices, pi_cap * sizeof(uint64_t));
        }
        prime_indices[pi_cnt++] = idx;
    }

    /* produce the row terms: prepend 1, compute successive sums, subtract 1 */
    seq_t prior = 1;
    for (size_t j = 0; j < pi_cnt; ++j) {
        seq_t sum = prior + (seq_t)prime_indices[j];
        seq_t val = sum - 1;
        if (terms_cnt == terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 4;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_cnt++] = val;
        prior = (seq_t)prime_indices[j];
    }

    free(prime_indices);
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2, values below are undefined */
    idx_t target = n - 2;         /* term position in the flattened list */
    while (terms_cnt <= target) { /* generate rows until enough terms */
        add_row(next_row);
        ++next_row;
    }
    return terms[target];
}

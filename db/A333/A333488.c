/*
 * A333488  First index of weakly decreasing prime quartets.
 * https://oeis.org/A333488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first index of the nth weakly decreasing prime quartet *
 *            (indexed from 1 for the first quart­et)
 * Range:     u8 n <= 43, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for primes and quartets */
static uint32_t *primes = NULL;       /* array of primes */
static size_t   primes_cnt = 0;       /* number of primes stored */
static idx_t    prime_limit = 0;      /* current sieve limit */

static idx_t *quartet_idx = NULL;    /* array of 1‑based indices of quartets */
static size_t quartet_cnt = 0;       /* number of quartets found so far */
static size_t quartet_cap = 0;       /* capacity of quartet_idx array */

/* Generate all primes up to 'limit' using a simple sieve (rebuilds array) */
static void generate_primes(idx_t limit)
{
    /* Allocate a bit array for the sieve */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) return;                       /* out of memory */

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (idx_t p = 2; (p * p) <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    /* Count primes to allocate the array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    uint32_t *new_primes = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
    if (!new_primes) { free(is_prime); return; }

    primes = new_primes;
    primes_cnt = cnt;

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[k++] = (uint32_t)i;

    free(is_prime);
    prime_limit = limit;
}

/* Scan current primes and store all weakly decreasing quartets */
static void compute_quartets(void)
{
    /* free old quartet array */
    free(quartet_idx);
    quartet_idx = NULL;
    quartet_cnt = 0;
    quartet_cap = 0;

    if (primes_cnt < 4) return;

    for (idx_t i = 0; i + 3 < primes_cnt; ++i) {
        uint32_t d1 = primes[i + 1] - primes[i];
        uint32_t d2 = primes[i + 2] - primes[i + 1];
        if (d1 < d2) continue;
        uint32_t d3 = primes[i + 3] - primes[i + 2];
        if (d2 < d3) continue;

        /* store 1‑based index (prime[0] is prime[1] = 2) */
        if (quartet_cnt == quartet_cap) {
            size_t new_cap = quartet_cap ? quartet_cap * 2 : 64;
            idx_t *tmp = (idx_t *)realloc(quartet_idx, new_cap * sizeof(idx_t));
            if (!tmp) break;   /* out of memory: stop growing */
            quartet_idx = tmp;
            quartet_cap = new_cap;
        }
        quartet_idx[quartet_cnt++] = i + 1;
    }
}

/* Ensure that at least 'target' quartets have been found */
static void ensure_quartets(idx_t target)
{
    while (quartet_cnt < target) {
        /* Decide next prime limit */
        idx_t new_limit = prime_limit ? prime_limit * 2 : 100000ull;
        if (new_limit < 100000u) new_limit = 100000u;

        generate_primes(new_limit);
        compute_quartets();
        /* loop continues if still not enough */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n<1, guard */

    ensure_quartets(n);

    return (seq_t)quartet_idx[n - 1];
}

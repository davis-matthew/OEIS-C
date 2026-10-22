/*
 * A379310  Number of nonsquarefree prime indices of n.
 * https://oeis.org/A379310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p^e‖n} e * (1 - I_squarefree(idx(p)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where idx(p) is the position of p in the prime list (first prime = 1). */
/* a(prime(k)) = 1 - A008966(k)          (k is not squarefree). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*   Global data – cached prime list and index square‑free flag */
/* ------------------------------------------------------------ */
static uint32_t *prime_list = NULL;   /* primes ≤ current_limit */
static size_t    prime_cnt  = 0;      /* number of stored primes   */
static uint64_t  current_limit = 0;   /* maximal n incorporated   */

static bool *index_sqfree = NULL;     /* true iff index is square‑free */
static size_t sqfree_size  = 0;       /* number of indices recorded  */

/* ------------------------------------------------------------ */
/*   Utility: Expand prime sieve up to at least 'limit'        */
/* ------------------------------------------------------------ */
static void expand_sieve(uint64_t limit)
{
    if (limit <= current_limit) return;           /* already done */

    /* Free old structures */
    free(prime_list);
    free(index_sqfree);

    if (limit < 2) {                              /* nothing to do */
        prime_list   = NULL;
        prime_cnt    = 0;
        current_limit = limit;
        return;
    }

    /* -------- 1. Count primes to allocate array -------- */
    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    size_t primes_estimate = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++primes_estimate;

    prime_list = (uint32_t *)malloc(primes_estimate * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    /* -------- 2. Actual sieve ------------------------------------------------ */
    prime_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_cnt++] = (uint32_t)i;
            if (i <= limit / i) {                     /* mark composites   */
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    current_limit = limit;

    /* -------- 3. Square‑free status of prime indices -------- */
    sqfree_size  = prime_cnt;
    index_sqfree = (bool *)calloc(sqfree_size, sizeof(bool));
    if (!index_sqfree) exit(EXIT_FAILURE);
    for (size_t i = 0; i < sqfree_size; ++i) index_sqfree[i] = true; /* all true */

    /* Mark indices that have a squared prime factor */
    for (size_t pi = 0; pi < prime_cnt && (uint64_t)prime_list[pi] * prime_list[pi] <= sqfree_size; ++pi) {
        uint64_t sq = (uint64_t)prime_list[pi] * prime_list[pi];
        for (uint64_t j = sq; j <= sqfree_size; j += sq)
            index_sqfree[(size_t)(j - 1)] = false;
    }

    free(is_comp);
}

/* ------------------------------------------------------------ */
/*   Main sequence function                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 by definition */

    /* Ensure prime data covers this n */
    expand_sieve((uint64_t)n);

    uint64_t temp = (uint64_t)n;
    seq_t    result = 0;

    /* Factorise using the prime list */
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (!index_sqfree[i])          /* i corresponds to index (i+1) */
                result += (seq_t)exp;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        /* Binary search for the prime in the list */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] < temp) lo = mid + 1;
            else hi = mid;
        }
        if (lo < prime_cnt && prime_list[lo] == temp) {
            if (!index_sqfree[lo]) result += (seq_t)1;
        }
    }

    return result;
}

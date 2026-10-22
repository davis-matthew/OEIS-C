/*
 * A355250  Largest prime appearing among the "middle parts" of the partitions
 *          of n into (exactly) 3 prime parts.
 * https://oeis.org/A355250
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum prime q such that n = p + q + r with p, q, r
 *            prime and p ≤ q ≤ r. The function implements a simple sieve to
 *            generate all primes up to n on the first call and decreases
 *            memory usage afterwards. It then searches the possible middle
 *            primes in descending order, testing for the existenc...
 * Range:     u8 n <= 515, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint8_t  *prime_mark = NULL;   /* 0/1 flags for primes */
static uint64_t *prime_list = NULL;   /* array of found primes */
static size_t    prime_cnt   = 0;      /* number of primes found   */
static idx_t     sieve_limit = 0;     /* current sieve upper bound */

/* Generate primes up to limit using a simple Eratosthenes sieve.  */
static void generate_primes(idx_t limit)
{
    /* Rebuild sieve if we need a higher limit.  */
    if (limit <= sieve_limit) return;

    /* Free old resources.  */
    free(prime_mark);
    free(prime_list);

    /* Allocate sieve storage.  */
    prime_mark = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!prime_mark) return;          /* out of memory – abort silently */

    /* Mark all numbers as prime initially (except 0,1 which remain 0).  */
    for (idx_t i = 2; i <= limit; ++i) prime_mark[i] = 1;

    /* Run the sieve.  */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_mark[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                prime_mark[j] = 0;
            }
        }
    }

    /* Reserve space for prime list; worst‑case count < limit/5 + 10.  */
    size_t est = (size_t)(limit / 5 + 10);
    prime_list = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!prime_list) return;          /* out of memory */

    /* Populate prime list.  */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (prime_mark[i]) {
            if (prime_cnt == est) {    /* enlarge if necessary */
                est *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, est * sizeof(uint64_t));
                if (!tmp) return;
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = i;
        }
    }

    sieve_limit = limit;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n < 6) return 0;            /* undefined for n < 6 */

    /* Build prime tables up to n.  */
    generate_primes(n);

    /* The largest middle prime cannot exceed (n – 2) / 2.  */
    const idx_t q_max_limit = (n >= 4) ? (n - 2) / 2 : 0;

    /* Scan middle primes from largest to smallest.  */
    for (size_t qi = prime_cnt; qi-- > 0;) {
        uint64_t q = prime_list[qi];
        if (q > q_max_limit) continue;          /* not allowed by r ≥ q */

        /* Calculate the maximum possible p for this q.  */
        if (n < 2 * q) continue;                /* no p can satisfy n - p - q ≥ q */
        idx_t p_limit = (n - 2 * q < q) ? (n - 2 * q) : q;
        if (p_limit < 2) continue;              /* p must be at least 2 */

        /* Check each prime p ≤ p_limit.  The array is sorted ascending.  */
        for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= p_limit; ++pi) {
            uint64_t p = prime_list[pi];
            idx_t r = n - p - q;
            if (r < q) continue;               /* ensure ordering p ≤ q ≤ r */
            if (prime_mark[r]) {                /* r is prime */
                return (seq_t)q;                /* found the largest middle prime */
            }
        }
    }

    /* According to Goldbach-type conjectures this should never happen
       for n ≥ 6, but we return 0 as a fallback.  */
    return 0;
}

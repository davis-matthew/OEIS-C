/*
 * A247978  Number of permutations of [n] that have no prime fixed points.
 * https://oeis.org/A247978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..pi(n)} (-1)^j * C(pi(n),j) * (n-j)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* pi(n)   = number of primes <= n */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- static data for the sieve ---------- */
static bool *is_prime = NULL;        /* [0..max_sieved] */
static size_t is_prime_cap = 0;      /* current capacity (max_sieved+1) */

static uint32_t *prime_list = NULL;  /* list of primes */
static size_t prime_cap = 0;         /* capacity of prime_list */
static size_t prime_cnt = 0;         /* number of primes stored */

static idx_t max_sieved = 0;         /* highest index already sieved */

/* ensure that the array is_prime[0..n] and prime_list contain all
   primes <= n.  No need to clear previous entries.  */
static void ensure_sieve(idx_t n)
{
    if (n <= max_sieved) return;          /* already done */

    size_t new_cap = (size_t)(n + 1);
    /* reallocate/initialise is_prime array */
    if (is_prime_cap < new_cap) {
        bool *tmp;
        if (is_prime == NULL) {
            tmp = (bool *)calloc(new_cap, sizeof(bool));
            if (!tmp) exit(EXIT_FAILURE);
        } else {
            tmp = (bool *)realloc(is_prime, new_cap * sizeof(bool));
            if (!tmp) exit(EXIT_FAILURE);
            /* initialise newly added entries to true */
            for (size_t i = is_prime_cap; i < new_cap; ++i)
                tmp[i] = true;
        }
        is_prime = tmp;
        is_prime_cap = new_cap;
    }

    /* mark 0 and 1 as non‑prime */
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    /* Sieve */
    for (size_t p = 2; p * p <= (size_t)n; ++p) {
        if (is_prime[p]) {
            for (size_t k = p * p; k <= (size_t)n; k += p)
                is_prime[k] = false;
        }
    }

    /* Update prime_list and prime_cnt */
    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_list) exit(EXIT_FAILURE);
        prime_cnt = 0;
    }

    for (size_t i = (size_t)max_sieved + 1; i <= (size_t)n; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    }

    max_sieved = n;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* make sure we know pi(n) */
    ensure_sieve(n);
    idx_t pi_n = (idx_t)prime_cnt;   /* pi(n) */

    /* factorial(n)! stored as current_fact, initially fact = n!   */
    seq_t current_fact = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        current_fact *= (seq_t)k;          /* (n)!   */

    seq_t comb = (seq_t)1;                 /* C(pi_n,0) */

    seq_t sum_even = (seq_t)0;
    seq_t sum_odd  = (seq_t)0;

    for (idx_t j = 0; j <= pi_n; ++j) {
        seq_t term = current_fact * comb;
        if ((j & 1) == 0)
            sum_even += term;
        else
            sum_odd += term;

        if (j < pi_n) {
            /* move to factorial(n-(j+1)) */
            current_fact /= (seq_t)(n - j);          /* divide by (n-j)  */
            /* update binomial coefficient */
            comb = comb * (seq_t)(pi_n - j) / (seq_t)(j + 1);
        }
    }

    return sum_even - sum_odd;   /* a(n) >= 0, so unsigned subtraction is safe */
}

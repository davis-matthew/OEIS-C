/*
 * A367791  Lesser of 2 successive tetraprimes (k, k+4) sandwiching three
 *          consecutive not squarefree numbers.
 * https://oeis.org/A367791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the lesser of successive tetraprimes k and k+4
 *            surrounding three consecutive non‑squarefree integers. Computed
 *            by an incremental search and memoisation
 * Range:     u8 none, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*---------------------------------------------------------------------*/
/*  Prime sieve – one‑time initialisation                              */
/*---------------------------------------------------------------------*/
#define MAX_PRIME_LIMIT 10000000   /* primes up to 10^7 (≈664k primes)   */
static bool primes_init = false;
static uint32_t primes_arr[700000];
static size_t primes_cnt = 0;
static bool prime_flag[MAX_PRIME_LIMIT+1];

static void init_primes(void)
{
    if (primes_init) return;

    memset(prime_flag, 1, sizeof(prime_flag));
    prime_flag[0] = prime_flag[1] = 0;
    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                prime_flag[j] = 0;
        }
    }
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (prime_flag[i]) primes_arr[primes_cnt++] = (uint32_t)i;

    primes_init = true;
}

/*---------------------------------------------------------------------*/
/*  Helper predicates                                                  */
/*---------------------------------------------------------------------*/
static bool is_tetraprime(uint64_t n)
{
    int distinct = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < primes_cnt &&
         (uint64_t)primes_arr[i] * (uint64_t)primes_arr[i] <= temp; ++i) {
        uint32_t p = primes_arr[i];
        if (temp % p == 0) {
            temp /= p;
            if (temp % p == 0) return false;          /* exponent > 1 */
            ++distinct;
            if (distinct > 4) return false;           /* too many primes */
        }
    }
    if (temp > 1) {
        ++distinct;                                   /* remaining prime */
        if (distinct > 4) return false;
    }
    return distinct == 4;
}

static bool not_squarefree(uint64_t n)
{
    for (size_t i = 0; i < primes_cnt &&
         (uint64_t)primes_arr[i] * (uint64_t)primes_arr[i] <= n; ++i) {
        uint32_t p = primes_arr[i];
        uint64_t sq = (uint64_t)p * (uint64_t)p;
        if (n % sq == 0) return true;
    }
    return false;
}

/*---------------------------------------------------------------------*/
/*  Storage for found terms                                            */
/*---------------------------------------------------------------------*/
static seq_t *terms = NULL;
static size_t terms_count = 0;
static size_t terms_cap   = 0;
static idx_t  last_checked = 0;

/*---------------------------------------------------------------------*/
/*  Ensure capacity for more stored terms                               */
/*---------------------------------------------------------------------*/
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    terms = tmp;
    terms_cap = new_cap;
}

/*---------------------------------------------------------------------*/
/*  Main function                                                       */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* one‑time initialisation */
    if (!primes_init) init_primes();

    if (terms == NULL) {
        terms = (seq_t*)malloc(64 * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
        terms_cap   = 64;
        terms_count = 0;
        last_checked = 0;
    }

    /* find terms until we have at least n of them */
    while (terms_count < n) {
        ++last_checked;               /* candidate k */
        uint64_t k = (uint64_t)last_checked;

        if (!is_tetraprime(k))               continue; /* need k tetraprime */
        if (!is_tetraprime(k + 4))           continue; /* need k+4 tetraprime */
        if (!not_squarefree(k + 1))          continue; /* k+1 not squarefree? */
        if (!not_squarefree(k + 2))          continue;
        if (!not_squarefree(k + 3))          continue;

        /* all conditions satisfied, store k */
        grow_terms(terms_count + 1);
        terms[terms_count++] = (seq_t)k;
    }

    /* return the n‑th term (1‑based) */
    return terms[n - 1];
}

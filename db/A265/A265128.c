/*
 * A265128  Nonprimes excluding numbers of the forms 2*p and p^i where p is a
 *          prime and i is a positive integer.
 * https://oeis.org/A265128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer (starting with 0 at index 1) that is
 *            neither prime nor of the form 2·p (p prime) nor a prime power
 *            p^i (i ≥ 1). The sequence begins 0, 1, 12, 15, 18, 20,…
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Sieve of Eratosthenes up to a fixed upper bound                       */
/* --------------------------------------------------------------------- */

#define SIEVE_LIMIT 200000   /* sufficient for n ≤ 10000 */

/* static data for the sieve */
static uint8_t *prime_flag = (uint8_t *)NULL;    /* 0 composite, 1 prime      */
static uint64_t *prime_list = (uint64_t *)NULL;  /* list of primes            */
static size_t prime_cnt  = 0;

/* once‑only initialization of the sieve */
static void build_sieve(void)
{
    if (prime_flag != NULL) return;                /* already built */

    prime_flag = (uint8_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint8_t));
    if (prime_flag == NULL) return;                /* out of memory – unlikely */

    /* initialise as all prime */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;

    /* Eratosthenes */
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_flag[j] = 0;
        }
    }

    /* build the prime list */
    /* allocate enough space – SIEVE_LIMIT is small, memory waste is fine */
    prime_list = (uint64_t *)malloc((SIEVE_LIMIT / 10 + 10) * sizeof(uint64_t));
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (prime_flag[i]) prime_list[prime_cnt++] = (uint64_t)i;
}

/* --------------------------------------------------------------------- */
/*  Helpers for determining the inclusion of an integer                  */
/* --------------------------------------------------------------------- */

/* primality test using the sieve (with trial division beyond the sieve) */
static bool is_prime(idx_t x)
{
    if (x <= SIEVE_LIMIT) return prime_flag[x] != 0;
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* test whether x is a prime power (x = p^k, k ≥ 1, p prime) */
static bool is_prime_power(idx_t x)
{
    if (x <= 1) return false;        /* 1 is not a prime power */

    /* find the first prime factor */
    idx_t tmp = x;
    uint64_t first_p = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            first_p = p;
            break;
        }
    }
    if (first_p == 0) return false;  /* x is prime or >SIEVE_LIMIT but prime */

    /* divide by that prime until it no longer divides */
    while (tmp % first_p == 0) tmp /= first_p;

    return tmp == 1;                /* only one prime factor present */
}

/* test whether x belongs to the sequence */
static bool is_candidate(idx_t x)
{
    if (x <= 1) return true;              /* 0 and 1 are allowed */

    if (is_prime(x)) return false;        /* exclude primes */

    if ((x & 1) == 0 && is_prime(x / 2)) return false;  /* 2·p */

    if (is_prime_power(x)) return false;  /* prime power (including primes) */

    return true;                          /* accepted */
}

/* --------------------------------------------------------------------- */
/*  Memoised storage for the generated terms (dynamic array)            */
/* --------------------------------------------------------------------- */

static seq_t *terms_arr  = (seq_t *)NULL;
static size_t terms_cap  = 0;
static size_t terms_cnt  = 0;
static idx_t  last_scanned = 0;

/* ensure capacity for the terms array */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    terms_arr = (seq_t *)realloc(terms_arr, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  The function required by the build system                            */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* defined only for n ≥ 1 */

    /* one‑time initialization */
    if (terms_arr == (seq_t *)NULL) {
        build_sieve();
        ensure_terms_cap(16);
        terms_arr[0] = (seq_t)0;       /* a(1) = 0 */
        terms_cnt = 1;
        last_scanned = 0;             /* next number to examine is 1 */
    }

    /* generate required terms if necessary */
    while (terms_cnt < n) {
        ++last_scanned;
        if (is_candidate(last_scanned)) {
            ensure_terms_cap(terms_cnt + 1);
            terms_arr[terms_cnt++] = (seq_t)last_scanned;
        }
    }

    return terms_arr[n - 1];
}

/*
 * A393065  Numbers k such that A020639(k') = A053669(k) and A053669(k') =
 *          A020639(k), where k' stands for the arithmetic derivative of k,
 *          A020639 returns the least prime factor of its argument, and
 *          A053669 is the least prime not dividing its argument.
 * https://oeis.org/A393065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 4441, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A393065 by searching for the numbers k such that
   A020639(A003415(k)) = A053669(k) and A053669(A003415(k)) = A020639(k). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/next_prime.h"   /* for next_prime */

/* ----------  Global data ---------------------------------- */
static idx_t   *spf_arr   = NULL;   /* smallest prime factor table up to LIMIT */
static size_t   spf_limit = 0;
static uint64_t *primes    = NULL;
static size_t   primes_cnt = 0;

static seq_t *terms      = NULL;   /* cached terms */
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static uint64_t current   = 2;     /* next candidate to test */

/* ----------  Prime table construction --------------------- */
static void init_prime_tables(void)
{
    if (spf_arr) return;                 /* already done */

    /* choose a limit that is more than enough for n ≤ 10 000 */
    const size_t LIMIT = 2000000;

    spf_arr = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));
    if (!spf_arr) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = (idx_t)i;
            if (i <= LIMIT / i) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    if (spf_arr[j] == 0) spf_arr[j] = (idx_t)i;
            }
        }
    }
    spf_limit = LIMIT;

    /* build list of primes up to LIMIT */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (spf_arr[i] == (idx_t)i) ++cnt;

    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t pos = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (spf_arr[i] == (idx_t)i) primes[pos++] = (uint64_t)i;

    primes_cnt = cnt;
}

/* ----------  Smallest prime factor ------------------------ */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return 1;
    if (n <= spf_limit) return spf_arr[n];

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;
        if (n % p == 0) return p;
    }
    return n;   /* n is prime */
}

/* ----------  Least prime not dividing n ------------------- */
static uint64_t missing_prime(uint64_t n)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (n % p != 0) return p;
    }
    /* all known primes divide n -> next prime after the table */
    return next_prime(primes[primes_cnt - 1]);
}

/* ----------  Arithmetic derivative ------------------------ */
static seq_t arithmetic_derivative(uint64_t n)
{
    seq_t sum = 0;
    uint64_t m = n;
    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        sum += (n / p) * exp;
    }
    return sum;
}

/* ----------  Test whether a candidate satisfies ----------- */
static bool satisfies(uint64_t k)
{
    uint64_t lpf_k     = smallest_prime_factor(k);
    uint64_t miss_k    = missing_prime(k);

    seq_t d_k          = arithmetic_derivative(k);
    uint64_t kprime    = (uint64_t)d_k;          /* k' fits in 64 bits for our range */

    uint64_t lpf_kprime  = smallest_prime_factor(kprime);
    uint64_t miss_kprime = missing_prime(kprime);

    return (lpf_kprime == miss_k) && (miss_kprime == lpf_k);
}

/* ----------  Ensure capacity of terms array --------------- */
static void ensure_capacity(size_t needed)
{
    if (needed <= terms_cap) return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 10000;
    while (new_cap < needed) new_cap *= 2;

    seq_t *new_terms;
#if defined(__cplusplus)
    new_terms = new seq_t[new_cap];
    for (size_t i = 0; i < terms_len; ++i) new_terms[i] = terms[i];
    delete[] terms;
#else
    new_terms = (seq_t*)calloc(new_cap, sizeof(seq_t));
    memcpy(new_terms, terms, terms_len * sizeof(seq_t));
    free(terms);
#endif
    terms      = new_terms;
    terms_cap  = new_cap;
}

/* ----------  Append one term to the cache ----------------- */
static void append_term(uint64_t val)
{
    ensure_capacity(terms_len + 1);
    terms[terms_len++] = (seq_t)val;
}

/* ----------  Main sequence function ---------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, never used */

    init_prime_tables();

    /* make sure 1 is the first term */
    if (terms_len == 0) {
        append_term(1);
        current = 2;
    }

    while (terms_len < (size_t)n) {
        if (satisfies(current))
            append_term(current);
        ++current;
    }
    return terms[n - 1];
}

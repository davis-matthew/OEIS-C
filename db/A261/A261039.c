/*
 * A261039  Numbers n that begin a sequence of 100 consecutive integers (n,
 *          n+1, ..., n+99) containing at least 25 primes.
 * https://oeis.org/A261039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth integer n for which the 100‑integer interval
   [n, n+99] contains at least 25 primes.  The routine builds a prime
   sieve upto a growing limit and slides a window over the range, 
   memoising the results so that nondeterministic ordering of calls
   is supported. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *prime_bool = NULL;       /* prime_bool[x] == 1 if x is prime */
static uint64_t prime_limit = 0;          /* current sieve upper bound */
static seq_t *valid_terms = NULL;         /* stored a(k) values */
static size_t valid_cap = 0, valid_cnt = 0; /* capacity and count of valid_terms */
static uint64_t next_start = 1;           /* next integer to examine */
static uint32_t cur_count = 0;            /* primes in current 100‑window */
static bool initialized = false;

/* Build a simple sieve of Eratosthenes up to 'limit'. */
static void build_sieve(uint64_t limit)
{
    free(prime_bool);
    prime_bool = (uint8_t *)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!prime_bool) exit(EXIT_FAILURE);
    memset(prime_bool, 1, (size_t)(limit + 1) * sizeof(uint8_t));
    if (limit >= 0) prime_bool[0] = 0;
    if (limit >= 1) prime_bool[1] = 0;
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (prime_bool[p]) {
            for (uint64_t j = p * p; j <= limit; j += p)
                prime_bool[j] = 0;
        }
    }
    prime_limit = limit;
}

/* Ensure prime_bool covers at least 'need'; extend if necessary. */
static void ensure_limit(uint64_t need)
{
    if (need <= prime_limit) return;
    uint64_t new_limit = prime_limit ? prime_limit * 2 + 100 : (need + 100);
    if (new_limit < need) new_limit = need + 100;
    build_sieve(new_limit);
}

/* Initialise static data structures on first use. */
static void initialise(void)
{
    if (initialized) return;
    /* Start with a reasonably large sieve so that few extensions are needed. */
    build_sieve(20000000ULL);
    valid_cap = 1024;
    valid_terms = (seq_t *)malloc(valid_cap * sizeof(seq_t));
    if (!valid_terms) exit(EXIT_FAILURE);
    initialized = true;
}

/* Append a new term, expanding storage as needed. */
static void add_valid(uint64_t v)
{
    if (valid_cnt >= valid_cap) {
        valid_cap = valid_cap ? valid_cap * 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(valid_terms, valid_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        valid_terms = tmp;
    }
    valid_terms[valid_cnt++] = (seq_t)v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but safe guard */
    initialise();

    /* Generate terms until we have at least n of them. */
    while (valid_cnt < (size_t)n) {
        if (next_start == 1) {        /* compute primes for first window [1,100] */
            cur_count = 0;
            for (uint64_t j = 1; j <= 100; ++j)
                cur_count += prime_bool[j];
        }

        if (cur_count >= 25)
            add_valid(next_start);

        /* Ensure we have primes up to next_start+100 for the sliding step. */
        ensure_limit(next_start + 100);

        /* Slide the window from [next_start, next_start+99]
           to [next_start+1, next_start+100].                 */
        cur_count -= prime_bool[next_start];          /* drop leftmost prime flag */
        cur_count += prime_bool[next_start + 100];    /* add new rightmost prime flag */

        ++next_start;
    }

    return valid_terms[(size_t)n - 1];
}

/*
 * A375711  Numbers k such that A013929(k+1) - A013929(k) = 3. In other words,
 *          the k-th nonsquarefree number is 3 less than the next
 *          nonsquarefree number.
 * https://oeis.org/A375711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute OEIS A375711: indices k such that consecutive
 *  nonsquarefree numbers differ by exactly 3.
 *
 *  The function generates nonsquarefree numbers by testing each
 *  integer for divisibility by a prime square.  It stores indices
 *  where the difference between consecutive nonsquarefree numbers
 *  equals 3.  Results are memoized so that subsequent calls may
 *  ask for any n≥1 in arbitrary order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/*----- State for prime generation -------------------------------------------*/
static uint32_t *prime_list = NULL; /* array of primes, 0‑based */
static size_t   prime_cap  = 0;     /* capacity of prime_list */
static size_t   prime_cnt  = 0;     /* number of primes stored   */
static uint32_t prime_lim  = 0;     /* largest number up to which primes are known */

/* Dynamically grow prime_list to hold at least new_cap entries. */
static void grow_primes(size_t new_cap)
{
    size_t cap = prime_cap;
    while (cap < new_cap) cap = cap ? cap << 1 : 8;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
    if (!tmp) { exit(1); }
    prime_list = tmp;
    prime_cap = cap;
}

/* Sieve of Eratosthenes up to limit (inclusive).  Adds all new primes
 * to prime_list.  Must be called with limit > prime_lim. */
static void sieve_primes_up_to(uint32_t limit)
{
    if (limit <= prime_lim) return;          /* nothing new to do */
    uint32_t size = limit + 1;
    uint8_t *is_prime = (uint8_t *)calloc(size, sizeof(uint8_t));
    if (!is_prime) exit(1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; i < size; ++i) is_prime[i] = 1;
    uint32_t r = (uint32_t)sqrt((double)limit);
    for (uint32_t p = 2; p <= r; ++p) {
        if (!is_prime[p]) continue;
        for (uint32_t m = p * p; m <= limit; m += p)
            is_prime[m] = 0;
    }
    /* count new primes and ensure capacity */
    for (uint32_t p = prime_lim + 1; p <= limit; ++p)
        if (is_prime[p]) {
            if (prime_cnt == prime_cap) grow_primes(prime_cnt + 1);
            prime_list[prime_cnt++] = p;
        }
    free(is_prime);
    prime_lim = limit;
}

/* Test whether x is nonsquarefree: return true if some prime square
 * divides x.  Uses the global prime_list, extending it if necessary. */
static bool is_nonsquarefree(uint64_t x)
{
    uint32_t sq_root = (uint32_t)sqrt((double)x);
    if (sq_root > prime_lim) sieve_primes_up_to(sq_root);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if (p > sq_root) break;
        uint64_t p2 = (uint64_t)p * p;
        if (x % p2 == 0) return true;
    }
    return false; /* squarefree */
}

/*----- State for sequence generation -------------------------------------------*/
static seq_t *seq_vals = NULL;  /* array of terms (indices k) */
static size_t seq_cap  = 0;     /* capacity of seq_vals */
static size_t seq_len  = 0;     /* number of terms stored */

static uint64_t cur_number = 1;      /* next natural number to test */
static uint64_t nsf_index = 0;       /* current index among nonsquarefree numbers */
static bool     prev_seen = false;   /* true if we have seen a nonsquarefree number */
static uint64_t prev_val  = 0;       /* value of previous nonsquarefree number */

/* Dynamically grow seq_vals to hold at least new_cap entries. */
static void grow_seq(size_t new_cap)
{
    size_t cap = seq_cap;
    while (cap < new_cap) cap = cap ? cap << 1 : 8;
    seq_t *tmp = (seq_t *)realloc(seq_vals, cap * sizeof(seq_t));
    if (!tmp) { exit(1); }
    seq_vals = tmp;
    seq_cap = cap;
}

/* Generate terms until seq_len >= target_len. */
static void generate_terms(size_t target_len)
{
    while (seq_len < target_len) {
        /* find next nonsquarefree number starting from cur_number */
        while (true) {
            uint64_t v = cur_number++;
            if (is_nonsquarefree(v)) {
                ++nsf_index;
                if (prev_seen) {
                    uint64_t diff = v - prev_val;
                    if (diff == 3) {
                        if (seq_len == seq_cap) grow_seq(seq_len + 1);
                        seq_vals[seq_len++] = (seq_t)(nsf_index - 1);
                    }
                }
                prev_val = v;
                prev_seen = true;
                break;            /* go to outer loop to check termination */
            }
        }
    }
}

/*----- The required OEIS function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n=0, return 0 */
    if (seq_len < (size_t)n) generate_terms((size_t)n);
    return seq_vals[n - 1];
}

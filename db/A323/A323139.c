/*
 * A323139  Integers that are not multiples of 6 and are the sum of two
 *          consecutive primes.
 * https://oeis.org/A323139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1412, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of two consecutive primes that are not multiples of 6 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ *
 * Internal helpers
 * ------------------------------------------------------------ */

/* integer sqrt for 64‑bit values, used only for prime testing */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ------------------------------------------------------------ *
 * State for primes and sums
 * ------------------------------------------------------------ */

static uint64_t *prime_list = NULL;          /* array of already found primes   */
static size_t     prime_cnt  = 0;            /* how many primes we have        */
static size_t     prime_cap  = 0;            /* allocated capacity for primes   */

static seq_t   *sum_list = NULL;             /* array of sums that satisfy the
                                                condition (not a multiple of 6)   */
static size_t    sum_cnt  = 0;               /* how many sums we have          */
static size_t    sum_cap  = 0;               /* allocated capacity for sums    */

static uint64_t  next_prime_candidate = 5;   /* next odd number to test for
                                                primality after 3            */

/* ------------------------------------------------------------ *
 * Grow the prime array if needed
 * ------------------------------------------------------------ */
static void grow_primes_if_needed(void)
{
    if (prime_cnt < prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();          /* out of memory */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* ------------------------------------------------------------ *
 * Grow the sum array if needed
 * ------------------------------------------------------------ */
static void grow_sums_if_needed(void)
{
    if (sum_cnt < sum_cap) return;
    size_t new_cap = sum_cap ? sum_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(sum_list, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    sum_list = tmp;
    sum_cap  = new_cap;
}

/* ------------------------------------------------------------ *
 * Generate the next prime number after prime_list[prime_cnt-1]
 * If prime_cnt < 2, this routine returns 2 or 3 accordingly.
 * ------------------------------------------------------------ */
static uint64_t next_prime(void)
{
    if (prime_cnt == 0) {                 /* first prime */
        return 2;
    }
    if (prime_cnt == 1) {                 /* second prime */
        return 3;
    }
    /* generate next odd candidate */
    uint64_t cand = next_prime_candidate;
    for (;;) {
        uint64_t limit = isqrt_u64(cand);
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q = prime_list[i];
            if (q > limit) break;
            if (cand % q == 0) { ok = false; break; }
        }
        if (ok) break;
        cand += 2;                         /* skip even numbers */
    }
    /* Update the next candidate for subsequent calls */
    next_prime_candidate = cand + 2;
    return cand;
}

/* ------------------------------------------------------------ *
 * Ensure that we have at least n terms of the sequence.
 * ------------------------------------------------------------ */
static void ensure_sums(idx_t n)
{
    while (sum_cnt < (size_t)n) {
        uint64_t p = next_prime();
        grow_primes_if_needed();
        prime_list[prime_cnt++] = p;

        /* If we have at least two primes, compute the sum of the
           two most recent primes and store it if it is not a multiple
           of 6. */
        if (prime_cnt > 1) {
            seq_t sum = (seq_t)prime_list[prime_cnt - 2] + (seq_t)prime_list[prime_cnt - 1];
            if ((sum % (seq_t)6) != 0) {
                grow_sums_if_needed();
                sum_list[sum_cnt++] = sum;
            }
        }
    }
}

/* ------------------------------------------------------------ *
 * Sequence entry point
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* lazy initialization of storage on first call */
    if (!sum_list) {
        sum_cap  = 16;
        sum_list = (seq_t *)malloc(sum_cap * sizeof(seq_t));
        if (!sum_list) abort();          /* out of memory */
        prime_cap  = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) abort();
    }
    ensure_sums(n);
    return sum_list[n - 1];
}

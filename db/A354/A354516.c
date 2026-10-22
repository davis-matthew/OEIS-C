/*
 * A354516  Smallest k such that m - gpf(m) = k has exactly n solutions m >=
 *          2, gpf = A006530; or -1 if no such k exists.
 * https://oeis.org/A354516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = smallest k such that the equation m - gpf(m) = k has
 *            exactly n solutions m ≥ 2. Here gpf is the greatest prime factor
 *            of m
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_K 10000000UL   /* upper bound for k to search up to */

static uint32_t *spf = NULL;          /* smallest prime factor table */
static seq_t   *min_k_for_count = NULL;/* minimal k for each count n */
static size_t   min_k_size = 0;       /* allocated size of min_k_for_count */
static idx_t    current_k = 0;        /* last k processed */
static idx_t    max_n_needed = 0;     /* maximum n we have to answer */

/* Build smallest‑prime‑factor sieve up to MAX_K */
static void build_spf(void)
{
    if (spf != NULL) return;
    spf = (uint32_t*)malloc((MAX_K + 1) * sizeof(uint32_t));
    if (!spf) return;  /* out of memory: shall not happen in tests */

    memset(spf, 0, (MAX_K + 1) * sizeof(uint32_t));
    for (idx_t i = 2; i <= MAX_K; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i > MAX_K / i) continue; /* avoid overflow of i*i */
            for (idx_t j = i * i; j <= MAX_K; j += i)
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i;
        }
    }
}

/* Ensure the min_k_for_count array can hold values up to n_needed. */
static void ensure_min_k_buffer(size_t n_needed)
{
    if (min_k_size > n_needed) return;
    size_t new_size = n_needed + 1;
    seq_t *new_buf = (seq_t*)realloc(min_k_for_count,
                                     new_size * sizeof(seq_t));
    if (!new_buf) return; /* cannot grow: memory failure */
    if (new_buf != min_k_for_count) { /* copy is done by realloc */
        min_k_for_count = new_buf;
    }
    /* initialise new slots to 0 (means not found yet) */
    for (size_t i = min_k_size; i < new_size; ++i)
        min_k_for_count[i] = 0;
    min_k_size = new_size;
}

/* Compute the number of valid primes p for a given k.
   That is: number of distinct prime divisors p of k such that
   all prime factors of (k/p + 1) are ≤ p. */
static size_t count_valid_primes(idx_t k)
{
    size_t cnt = 0;
    idx_t tmp = k;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        /* process prime p once */
        idx_t t = k / p + 1;
        idx_t ttmp = t;
        uint32_t max_factor = 1;
        while (ttmp > 1) {
            uint32_t q = spf[ttmp];
            if (q > max_factor) max_factor = q;
            while (ttmp % q == 0) ttmp /= q;
        }
        if (max_factor <= p) cnt++;
        while (tmp % p == 0) tmp /= p;
    }
    return cnt;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)1;

    build_spf();                     /* initialise sieve once */
    ensure_min_k_buffer((size_t)n);  /* make sure buffer big enough */

    /* If already know answer, return it */
    if (min_k_for_count[n] != 0)
        return min_k_for_count[n];

    /* Update required maximum n */
    if (n > max_n_needed) max_n_needed = n;

    /* Process k from the point we left off */
    for (idx_t k = current_k + 1; k <= MAX_K; ++k) {
        size_t c = count_valid_primes(k);
        if (c <= max_n_needed && min_k_for_count[c] == 0) {
            min_k_for_count[c] = (seq_t)k;
        }
        if (min_k_for_count[n] != 0) {
            current_k = k;
            return min_k_for_count[n];
        }
    }

    /* If we exhaust the limit and still haven't found, return -1. */
    current_k = MAX_K;
    return (seq_t)(-1);
}

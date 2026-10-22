/*
 * A357864  Numbers whose prime indices have strictly decreasing run-sums.
 *          Heinz numbers of the partitions counted by A304430.
 * https://oeis.org/A357864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A357864: numbers whose prime indices have strictly decreasing run‑sums. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/prime_pi.h"   /* provides prime_pi */

static uint64_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static size_t    prime_cap  = 0;
static uint64_t  prime_limit = 0;

/* Generate all primes up to LIMIT (inclusive) using the sieve of Eratosthenes.
   The list is stored in prime_list and prime_cnt.  The routine is called
   only once during program execution. */
static void generate_primes_upto(uint64_t limit)
{
    uint8_t *mark = (uint8_t *)malloc(limit + 1);
    if (!mark) abort();
    memset(mark, 0, limit + 1);

    size_t cap = limit + 1;                     /* worst‑case number of primes */
    uint64_t *list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!list) abort();
    size_t cnt = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!mark[i]) {
            if (cnt == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
                if (!tmp) abort();
                list = tmp;
            }
            list[cnt++] = i;
            if (i <= limit / i) {
                for (uint64_t j = i * i; j <= limit; j += i) mark[j] = 1;
            }
        }
    }

    free(mark);
    if (prime_list) free(prime_list);
    prime_list = list;
    prime_cnt  = cnt;
    prime_cap  = cap;
    prime_limit = limit;
}

/* Test whether NUM satisfies the strictly decreasing run-sum property
   of A357864.  NUM is assumed to fit into idx_t (uint64_t).  For a
   factor p^e the run sum is e * index(p).  The sequence of such sums
   must be strictly decreasing when the primes are processed in
   ascending order. */
static bool valid_candidate(idx_t num)
{
    if (num == 1) return true;

    idx_t last_sum = (idx_t)-1;      /* sentinel for the first run */
    idx_t temp = num;

    while (temp > 1) {
        uint64_t p = 0;
        size_t i = 0;
        int exp = 0;

        /* Find the smallest prime factor of temp. */
        for (i = 0; i < prime_cnt; ++i) {
            uint64_t prime = prime_list[i];
            if (prime > temp / prime) break;     /* prime > sqrt(temp) */
            if (temp % prime == 0) {
                p = prime;
                break;
            }
        }

        if (p == 0) {          /* temp itself is prime */
            p = temp;
            exp = 1;
            temp = 1;
        } else {
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
        }

        idx_t idxp = (idx_t)prime_pi(p);   /* 1‑based index of prime p */
        idx_t cur_sum = (idx_t)exp * idxp;

        if (last_sum != (idx_t)-1 && last_sum <= cur_sum) return false;
        last_sum = cur_sum;
    }
    return true;
}

/* Storage for sequence terms discovered so far. */
static seq_t *seq_terms = NULL;
static size_t seq_cap   = 0;
static idx_t  seq_len   = 0;
static idx_t  next_num  = 1;   /* next positive integer to examine */

/* Return the n‑th term of A357864.  Terms are cached so that repeated
   calls are efficient. */
seq_t a(idx_t n)
{
    /* Initialise the prime list once. */
    if (prime_cnt == 0) {
        generate_primes_upto(2000000);   /* should cover all candidates used. */
    }

    while (seq_len < n) {
        if (valid_candidate(next_num)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq_terms = tmp;
                seq_cap = new_cap;
            }
            seq_terms[seq_len++] = (seq_t)next_num;
        }
        ++next_num;
    }
    return seq_terms[n - 1];
}

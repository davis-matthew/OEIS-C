/*
 * A370139  Primes p such that the sums of three, five, and seven consecutive
 *          primes starting with p are prime.
 * https://oeis.org/A370139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that the sums of the first 3, 5 and
 *            7 consecutive primes starting with p are all prime
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"          /* is_prime(idx_t) */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t, uint64_t **, size_t *) */

static uint64_t *g_primes = NULL;          /* array of primes computed so far */
static size_t     g_prime_cnt = 0;          /* number of primes in g_primes      */
static idx_t      g_prime_limit = 0;        /* current sieving upper bound       */
static idx_t      g_next_prime_idx = 0;     /* index of next prime to test      */

static seq_t *g_seq = NULL;                /* cached sequence terms            */
static idx_t  g_seq_len = 0;                /* number of terms cached so far    */
static idx_t  g_seq_cap = 0;                /* allocated capacity of g_seq       */

/* Extend the prime table until it contains at least (need+1) primes. */
static void extend_primes_to(idx_t need_plus_one)
{
    if (g_prime_cnt > need_plus_one) return;                   /* already enough */

    idx_t new_limit = (g_prime_limit == 0) ? 1000000 : g_prime_limit * 2;
    while (true) {
        uint64_t *tmp = NULL; size_t tmpcnt = 0;
        build_primes_upto(new_limit, &tmp, &tmpcnt);
        if (tmpcnt > need_plus_one) {
            free(g_primes);
            g_primes = tmp;
            g_prime_cnt = tmpcnt;
            g_prime_limit = new_limit;
            return;
        }
        free(tmp);
        new_limit *= 2;                                        /* try again */
    }
}

/* Store a new term in the cache, growing the array if necessary. */
static void cache_term(seq_t val)
{
    if (g_seq_len >= g_seq_cap) {
        idx_t new_cap = (g_seq_cap == 0) ? 128 : g_seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(g_seq, new_cap * sizeof(seq_t));
        g_seq = tmp;
        g_seq_cap = new_cap;
    }
    g_seq[g_seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1, guard */

    if (g_seq_len < n) {                      /* generate terms up to n */
        /* ensure we have enough primes to examine */
        extend_primes_to(g_next_prime_idx + 6);

        while (g_seq_len < n) {
            /* ensure we still have 7 consecutive primes from the current index */
            if (g_next_prime_idx + 6 >= g_prime_cnt) {
                extend_primes_to(g_next_prime_idx + 6);
            }

            /* retrieve the first 7 consecutive primes starting at current index */
            const uint64_t *p = &g_primes[g_next_prime_idx];
            seq_t sum3 = (seq_t)p[0] + p[1] + p[2];
            seq_t sum5 = sum3 + p[3] + p[4];
            seq_t sum7 = sum5 + p[5] + p[6];

            if (is_prime((idx_t)sum3) && is_prime((idx_t)sum5) && is_prime((idx_t)sum7)) {
                cache_term((seq_t)p[0]);                /* store the prime itself */
            }

            g_next_prime_idx++;                      /* move to next candidate */
            extend_primes_to(g_next_prime_idx + 6);
        }
    }

    return g_seq[n - 1];
}

/*
 * A358580  Difference between the number of leaves and the number of internal
 *          (non-leaf) nodes in the rooted tree with Matula-Goebel number n.
 * https://oeis.org/A358580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1 a(n) = sum_{p | n} a(index_of_prime(p)) - 1 for n > 1
 *            where each prime factor p of n (with multiplicity) is replaced
 *            by the 1‑based index of the prime p in the ordered list of
 *            primes. The recurrence follows from the definition of leaves and
 *            internal nodes in the Matula–Goebel tree
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Prime infrastructure                                         */
/* ------------------------------------------------------------ */
static uint32_t *primes = NULL;          /* list of discovered primes      */
static size_t primes_cnt = 0;            /* number of primes found         */
static size_t primes_cap = 0;            /* capacity of the primes array   */
static idx_t   max_prime_checked = 0;    /* maximum integer that has been
                                           checked for primality       */
static idx_t  *prime_index_arr = NULL;   /* prime → 1‑based index, 0 if not prime */
static size_t  prime_index_cap = 0;      /* capacity of prime_index_arr */

/* Ensure that primes up to 'limit' and the index array are available. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= max_prime_checked) return;

    /* Grow the prime_index_arr if necessary. */
    if (limit >= prime_index_cap) {
        size_t new_cap = prime_index_cap ? prime_index_cap << 1 : 1;
        while (new_cap <= limit) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(prime_index_arr,
                                      new_cap * sizeof(idx_t));
        /* Zero‑initialize the newly added entries. */
        for (size_t i = prime_index_cap; i < new_cap; ++i) tmp[i] = 0;
        prime_index_arr = tmp;
        prime_index_cap = new_cap;
    }

    /* Check each integer from max_prime_checked+1 up to limit. */
    for (idx_t num = max_prime_checked + 1; num <= limit; ++num) {
        bool is_prime = true;
        if (num < 2) is_prime = false;
        else {
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint32_t p = primes[i];
                if ((idx_t)p * (idx_t)p > num) break;
                if (num % p == 0) { is_prime = false; break; }
            }
        }

        if (is_prime) {
            /* Append to the primes list. */
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap << 1 : 1;
                uint32_t *tmp = (uint32_t *)realloc(primes,
                                                    new_cap * sizeof(uint32_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (uint32_t)num;

            /* Record the 1‑based index of this prime. */
            prime_index_arr[num] = primes_cnt;
        }
    }

    max_prime_checked = limit;
}

/* ------------------------------------------------------------ */
/* Cache for a(n)                                               */
/* ------------------------------------------------------------ */
static seq_t *a_cache  = NULL;      /* holds computed a(n) values       */
static bool  *computed = NULL;      /* true if the slot is valid       */
static idx_t  cache_cap = 0;        /* cardinality of the two arrays    */

static void ensure_cache_size(idx_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= needed) new_cap <<= 1;

    seq_t  *tmp_cache  = (seq_t *)realloc(a_cache,
                                          new_cap * sizeof(seq_t));
    bool   *tmp_comp   = (bool  *)realloc(computed,
                                          new_cap * sizeof(bool));

    a_cache   = tmp_cache;
    computed  = tmp_comp;

    for (size_t i = cache_cap; i < new_cap; ++i) {
        computed[i] = false;       /* value not yet stored         */
    }
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Recursive evaluation of a(n)                                */
/* ------------------------------------------------------------ */
static seq_t compute_a(idx_t n)
{
    if (n < cache_cap && computed[n]) return a_cache[n];
    if (n == 1) {
        seq_t val = (seq_t)1;
        ensure_cache_size(2);
        a_cache[1]   = val;
        computed[1]  = true;
        return val;
    }

    /* Ensure that we have enough primes to factor n and look up child indices. */
    ensure_primes_up_to(n);

    idx_t tmp = n;
    seq_t sum = (seq_t)0;

    /* Factor n by trial division using the primes list. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        while (tmp % p == 0) {
            idx_t child = prime_index_arr[p];
            sum += compute_a(child);
            tmp /= p;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        idx_t child = prime_index_arr[tmp];
        sum += compute_a(child);
    }

    seq_t result = sum - (seq_t)1;   /* subtract the root's internal node */
    ensure_cache_size(n + 1);
    a_cache[n]   = result;
    computed[n]  = true;
    return result;
}

/* ------------------------------------------------------------ */
/* Public entry point                                           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    return compute_a(n);
}

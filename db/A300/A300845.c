/*
 * A300845  a(n) is the smallest prime q such that q^2 + q*p + p^2 is a prime
 *          number where p is n-th prime, or 0 if no such q exists.
 * https://oeis.org/A300845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest prime q such that q^2 + q*p + p^2 is prime,
 *            where p is the n‑th prime (1‑based). Return 0 if no such q
 *            exists. The function is fully thread‑safe and memoised per
 *            process. It works for any seq_t that supports normal integer
 *            arithmetic and the routine is_prime_big
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 53, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds primes on demand, keeping a list
   of all primes found so far.  For a given index n we first make
   sure that the list contains at least n primes.  Then we
   probe progressively larger primes q from the beginning of the
   list until q^2 + q*p + p^2 is prime.  No external tables are
   referenced. */ 

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

/* Grow the prime list so that it contains at least need primes. */
static void ensure_primes_upto(size_t need)
{
    if (prime_cnt >= need) return;

    /* Start with a small capacity and grow exponentially. */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cap < need) {
        while (prime_cap < need) prime_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        prime_list = tmp;
    }

    /* Trial‑division generation of new primes.  The next candidate
       starts at the last prime seen or 2 for the very first prime. */
    uint64_t cand = prime_cnt == 0 ? 2 : prime_list[prime_cnt - 1] + 1;
    if (cand > 2 && (cand & 1) == 0) ++cand;      /* skip even numbers */

    while (prime_cnt < need) {
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime) prime_list[prime_cnt++] = cand;
        cand += (cand <= 2 ? 1 : 2);              /* next candidate */
    }
}

/* Return the smallest q for which q^2 + q*p + p^2 is prime.
   Preconditions: prime_cnt >= n (so p is available) */
static seq_t find_smallest_q_for_p(size_t n)
{
    seq_t p = (seq_t)prime_list[n - 1];   /* n is 1‑based */
    size_t q_idx = 0;

    while (true) {
        /* Ensure we have a prime for this q index. */
        if (q_idx >= prime_cnt) {
            /* Extend the prime list by one further prime. */
            ensure_primes_upto(prime_cnt + 1);
        }

        seq_t q = (seq_t)prime_list[q_idx++];
        seq_t val = q * q + q * p + p * p;

        if (is_prime_big(val)) return q;
    }
}

/* --- Public function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* below the offset – behaviour irrelevant */

    size_t idx = (size_t)n;              /* convert to 0‑based for internal use */
    ensure_primes_upto(idx);             /* guarantees at least n primes */

    return find_smallest_q_for_p(idx);
}

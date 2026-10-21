/*
 * A279458  Numbers k such that number of distinct primes dividing k is even
 *          and number of prime divisors (counted with multiplicity) of k is
 *          even.
 * https://oeis.org/A279458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A279458 – numbers k such that 
   ω(k) (number of distinct prime divisors) and Ω(k) (total number of prime
   divisors counted with multiplicity) are both even.  The routine
   builds the sequence lazily by testing successive integers, factoring
   each using a prime list that is grown incrementally.  The result of
   a(1) is 1, matching the required offset. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- helpers for prime generation (incremental sieve) ---------- */
static uint64_t *prime_buf   = NULL;   /* stored primes */
static size_t   prime_cap   = 0;
static size_t   prime_len   = 0;
static uint64_t next_prime_candidate = 3;   /* next odd to test */

/* append a prime to the prime buffer */
static void add_prime(uint64_t p) {
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_buf, new_cap * sizeof(uint64_t));
        prime_buf = tmp;
        prime_cap = new_cap;
    }
    prime_buf[prime_len++] = p;
}

/* grow the prime buffer until all primes ≤ limit are present */
static void ensure_primes_up_to(uint64_t limit) {
    if (prime_len == 0) {          /* initialise with the first prime */
        add_prime(2);
        /* next odd to test after 2 is 3 */
    }
    while (next_prime_candidate <= limit) {
        bool is_prime = true;
        uint64_t cand = next_prime_candidate;
        /* trial division by known primes up to √cand */
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_buf[i];
            if (p > cand / p) break;          /* p*p > cand */
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) add_prime(cand);
        next_prime_candidate += 2;            /* skip even numbers */
    }
}

/* ---------- factorisation helper -------------------------------------- */
/* compute ω(n) and Ω(n); results are returned via pointers.            */
static void factor_counts(uint64_t n, int *omega, int *bigomega) {
    *omega    = 0;
    *bigomega = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_len && prime_buf[i] <= temp / prime_buf[i]; ++i) {
        uint64_t p = prime_buf[i];
        if (temp % p == 0) {
            ++(*omega);
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            *bigomega += exp;
        }
    }
    if (temp > 1) {  /* remaining prime factor > sqrt(original n) */
        ++(*omega);
        ++(*bigomega);
    }
}

/* ---------- cache of sequence terms ----------------------------------- */
static seq_t *term_buf   = NULL;     /* stored sequence values */
static size_t term_cap   = 0;
static size_t term_len   = 0;
static uint64_t checked = 0;         /* largest integer examined so far */

/* append a term to the term cache */
static void add_term(seq_t val) {
    if (term_len == term_cap) {
        size_t new_cap = term_cap == 0 ? 16 : term_cap * 2;
        seq_t *tmp = (seq_t *)realloc(term_buf, new_cap * sizeof(seq_t));
        term_buf  = tmp;
        term_cap  = new_cap;
    }
    term_buf[term_len++] = val;
}

/* ----------------------------------------------------------------------- */
/*  a(n):  the n-th term (a(1) = 1)                                         */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* target zero‑based index for the n-th term */
    idx_t target = n - 1;

    /* generate terms until the cache contains the requested one       */
    while (term_len <= target) {
        /* examine the next integer */
        ++checked;

        /* ensure we have all primes needed for factoring this number  */
        ensure_primes_up_to(checked);

        /* factorise and test parity conditions                       */
        int omega, bigomega;
        factor_counts(checked, &omega, &bigomega);
        if ( ((omega    & 1) == 0) &&
             ((bigomega & 1) == 0) ) {
            add_term((seq_t)checked);
        }
    }

    return term_buf[target];
}

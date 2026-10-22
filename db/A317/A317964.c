/*
 * A317964  Prime numbers in the lexicographically earliest sequence of
 *          positive integers whose prime indices are not already in the
 *          sequence (A304360).
 * https://oeis.org/A317964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 4513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A317964 – Prime numbers in the lexicographically earliest sequence
 *  of positive integers whose prime indices are not already in the sequence
 *  (A304360).  The sequence is generated on demand: starting from 1,
 *  each natural number is tested in ascending order; if none of the
 *  indices of its prime factors have appeared previously in the
 *  constructed sequence S, the number is appended to S.  The function
 *  returns the nth prime occurring in S (a(1)=2, a(2)=5, …).  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes.h"

/* static data structures (computed once) */
static bool initialized = false;
static const size_t LIMIT = 2000000;          /* upper bound for precomputation */
static idx_t *spf = NULL;                     /* smallest prime factor array */
static idx_t *primes_arr = NULL;              /* primes up to LIMIT */
static size_t primes_cnt = 0;                 /* number of primes */
static idx_t *pos_of_prime = NULL;            /* prime value -> its 1‑based index */
static char *inS = NULL;                      /* flags: 1 if number is in S */
static seq_t *prime_in_S = NULL;              /* primes that belong to S */
static size_t prime_in_S_size = 0;            /* current count of primes in S */
static size_t prime_in_S_cap   = 0;            /* allocated capacity for prime_in_S */
static idx_t next_candidate = 2;              /* next number to test */

/* initialise lookup tables */
static void init_sequence_tables(void)
{
    /* smallest prime factor array */
    build_spf(LIMIT, &spf);

    /* list of primes up to LIMIT */
    build_primes(LIMIT, &primes_arr, &primes_cnt);

    /* map each prime to its position (1‑based) */
    pos_of_prime = (idx_t*)calloc((LIMIT + 1), sizeof(idx_t));
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes_arr[i];
        pos_of_prime[p] = (idx_t)(i + 1);
    }

    /* mark numbers already in the sequence S */
    inS = (char*)calloc((LIMIT + 1), sizeof(char));

    /* dynamic array for primes that are in S */
    prime_in_S_cap = 64;
    prime_in_S = (seq_t*)malloc(prime_in_S_cap * sizeof(seq_t));
    prime_in_S_size = 0;
    next_candidate   = 2;

    initialized = true;
}

/*  ----------------------------------------------------------------------  */
/*  The required function                                               */
/*  ----------------------------------------------------------------------  */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence_tables();

    if (n == (idx_t)0)
        return (seq_t)0;      /* undefined for n=0, but should not be requested */

    /* generate sequence until we have at least n primes in S */
    while (prime_in_S_size < (size_t)n) {
        idx_t candidate = next_candidate;

        /* safety guard: ensure we stay within precomputed limits */
        if (candidate > LIMIT) {
            /* In the unlikely event the limit is exceeded, abort.
               For the tested range (n up to ~1e4) LIMIT=2,000,000
               is ample. */
            break;
        }

        /* check candidate: none of its prime‑index positions must already
         * appear in S (i.e., be present as a number in S). */
        bool ok = true;
        idx_t temp = candidate;
        idx_t last_p = 0;
        while (temp > 1 && ok) {
            idx_t p = spf[temp];
            if (p != last_p) {                /* distinct prime factor */
                idx_t idx = pos_of_prime[p];   /* position of p in prime list */
                if (idx > 0 && inS[idx]) {     /* index already used in S */
                    ok = false;
                    break;
                }
                last_p = p;
            }
            temp /= p;
        }

        if (ok) {
            /* accept candidate into S */
            inS[candidate] = 1;

            /* if it's a prime, store it for output */
            if (candidate == spf[candidate]) {
                if (prime_in_S_size == prime_in_S_cap) {
                    prime_in_S_cap <<= 1;
                    prime_in_S = (seq_t*)realloc(prime_in_S,
                                                 prime_in_S_cap * sizeof(seq_t));
                }
                prime_in_S[prime_in_S_size++] = (seq_t)candidate;
            }
        }
        ++next_candidate;
    }

    /* S is 1‑indexed; return the n‑th prime in the list */
    return prime_in_S[(size_t)n - 1];
}

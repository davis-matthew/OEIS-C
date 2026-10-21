/*
 * A278785  Primes p such that prime(p-2) + prime(p-1) + prime(p+1) is prime.
 * https://oeis.org/A278785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that *
 *            prime(p-2)+prime(p-1)+prime(p+1) is prime * (here bold
 *            “prime(k)” denotes the k‑th prime number). * The function
 *            generates primes sequentially, checks the * condition for each
 *            prime whose next prime is known, * and memoises the qualifying
 *            primes for subsequent...
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External helpers required by this file */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/*------------------------------------------------------------------*/
/*  Internal state for memoisation                                  */
/*------------------------------------------------------------------*/
static uint64_t *prime_list = NULL;   /* primes found so far                */
static size_t   prime_cnt  = 0;        /* number of primes stored           */
static size_t   prime_cap  = 0;        /* capacity of prime_list             */

static seq_t   *term_list  = NULL;     /* sequence terms computed so far    */
static size_t   term_cnt   = 0;        /* number of terms stored            */
static size_t   term_cap   = 0;        /* capacity of term_list             */

static uint64_t next_candidate = 2;    /* next number to test as a prime    */

/*------------------------------------------------------------------*/
/*  Utility: append a prime to the prime list                       */
/*------------------------------------------------------------------*/
static void add_next_prime(void)
{
    uint64_t cand = next_candidate;

    /* Find the next prime (trial division using known primes) */
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {  /* cand is prime */
            /* Grow the prime list if required */
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                prime_list = (uint64_t *)realloc(prime_list,
                              prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = cand;

            /* Advance to the next candidate */
            if (cand == 2)
                next_candidate = 3;
            else
                next_candidate = cand + 2;   /* skip evens */

            return;
        }

        /* cand is composite – try the next odd number */
        cand += 2;
        if (cand < 3) cand = 3;      /* safety for wrap‑around */
        next_candidate = cand;
    }
}

/*------------------------------------------------------------------*/
/*  Utility: append a term to the term cache                       */
/*------------------------------------------------------------------*/
static void add_term(uint64_t value)
{
    if (term_cnt == term_cap) {
        term_cap = (term_cap == 0) ? 16 : term_cap * 2;
        term_list = (seq_t *)realloc(term_list,
                        term_cap * sizeof(seq_t));
    }
    term_list[term_cnt++] = (seq_t)value;
}

/*------------------------------------------------------------------*/
/*  Main sequence function                                          */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them */
    while (term_cnt < n) {
        add_next_prime();          /* add a new prime */

        /* We can evaluate the condition for the prime
           just before the one we just added (i.e., the
           prime at index prime_cnt-2) once we have at
           least 4 primes in total (to access idx-2 and idx+1). */
        if (prime_cnt >= 4) {
            /* indices: 0‑based
               candidate prime: prime_list[prime_cnt-2]
               sum uses indices prime_cnt-4, prime_cnt-3, prime_cnt-1 */
            uint64_t sum = prime_list[prime_cnt-4] +
                           prime_list[prime_cnt-3] +
                           prime_list[prime_cnt-1];

            if (is_prime((idx_t)sum))
                add_term(prime_list[prime_cnt-2]);   /* qualifying prime */
        }
    }
    return term_list[n - 1];     /* 1‑based indexing */
}

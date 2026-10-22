/*
 * A381044  Primes prime(k) followed by a gap, prime(k+1)-prime(k), smaller
 *          than the local geometric average gap between consecutive primes:
 *          log(prime(k))/e^(gamma).
 * https://oeis.org/A381044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p_k such that log(p_{k+1} - p_k) <
 *            log(log(p_k)) - γ (γ = Euler's constant) Sequence is indexed
 *            starting at 1. The implementation generates primes sequentially
 *            and stores all qualifying primes in a memoised array. The
 *            computation is deterministic and works for any order of c...
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t) */

static uint64_t *prime_arr = NULL;   /* array of primes found          */
static size_t prime_cnt  = 0;         /* number of primes stored        */
static size_t prime_cap  = 0;         /* allocated capacity             */

static seq_t *term_arr    = NULL;    /* array of sequence terms        */
static size_t term_cnt    = 0;        /* number of terms stored         */
static size_t term_cap    = 0;        /* allocated capacity             */

static uint64_t prev_prime = 0;       /* last prime processed           */
static uint64_t next_candidate = 2;   /* next integer to test for prime*/

static const long double gamma_val = 0.57721566490153286060651209L;

/* Ensure prime array has at least 'min_cap' slots. */
static void prime_ensure_cap(size_t min_cap) {
    if (prime_cap < min_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 64;
        while (new_cap < min_cap) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(prime_arr, new_cap * sizeof(uint64_t));
        prime_arr = tmp;
        prime_cap = new_cap;
    }
}

/* Ensure term array has at least 'min_cap' slots. */
static void term_ensure_cap(size_t min_cap) {
    if (term_cap < min_cap) {
        size_t new_cap = term_cap ? term_cap << 1 : 64;
        while (new_cap < min_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(term_arr, new_cap * sizeof(seq_t));
        term_arr = tmp;
        term_cap = new_cap;
    }
}

/* Generate terms until we have at least 'n' terms. */
static void generate_terms(idx_t n) {
    while (term_cnt < (size_t)n) {
        /* Find next prime */
        while (true) {
            uint64_t cand = next_candidate++;
            if (is_prime(cand)) {
                /* Store the newly found prime */
                prime_ensure_cap(prime_cnt + 1);
                prime_arr[prime_cnt++] = cand;

                if (prev_prime) {
                    uint64_t gap = cand - prev_prime;
                    long double left  = logl((long double)gap);
                    long double right = logl(logl((long double)prev_prime)) - gamma_val;
                    if (left < right) {
                        term_ensure_cap(term_cnt + 1);
                        term_arr[term_cnt++] = (seq_t)prev_prime;
                    }
                }
                prev_prime = cand;
                break;
            }
        }
    }
}

/* External function required by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but harmless */
    generate_terms(n);
    return term_arr[n - 1];
}

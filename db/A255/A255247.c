/*
 * A255247  Fundamental positive solution x = x2(n) of the second class of the
 *          Pell equation x^2 - 2*y^2 = -A001132(n), n>=1 (primes congruent to
 *          {1,7} mod 8).
 * https://oeis.org/A255247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = second smallest positive integer x such that
 *          x^2 - 2*y^2 = -p,
 *          where p is the n-th prime that is ≡ 1 or 7 (mod 8).
 *
 * The function generates primes on demand (using next_prime) and
 * finds the second solution for each such prime by a simple
 * search over increasing x.  The first solution (the fundamental
 * proper one) is skipped; the next is returned.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"

/* --- static storage for primes of the form 1 or 7 mod 8 ------------------- */
static uint64_t *prime_list   = NULL;      /* primes with p % 8 == 1 or 7   */
static size_t   prime_cap    = 0;          /* allocated capacity            */
static size_t   prime_len    = 0;          /* number of primes stored      */
static uint64_t last_gen_prime = 2;        /* last prime produced by next_prime */

/* --- static storage for cached a(n) values -------------------------------- */
static seq_t *value_cache   = NULL;         /* cache of a(n) values           */
static size_t value_cap     = 0;            /* allocated capacity            */
static size_t value_len     = 0;            /* number of values cached        */

/* -------------------------------------------------------------------------- */
/* Ensure prime_list capacity.  Must cast for C++ compatibility.            */
static void ensure_prime_capacity(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* -------------------------------------------------------------------------- */
/* Ensure value_cache capacity.  Must cast for C++ compatibility.           */
static void ensure_value_capacity(size_t needed)
{
    if (needed <= value_cap) {
        if (needed > value_len) value_len = needed;
        return;
    }
    size_t new_cap = value_cap ? value_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(value_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    value_cache = tmp;
    /* initialise new slots to zero so that zero can be used as sentinel */
    for (size_t i = value_len; i < new_cap; ++i)
        value_cache[i] = (seq_t)0;
    value_cap = new_cap;
    value_len = needed;
}

/* -------------------------------------------------------------------------- */
/* Find the second smallest positive solution of x^2 - 2*y^2 = -p.           */
static seq_t find_second_solution(uint64_t p)
{
    seq_t found = 0;      /* count of solutions found so far */
    seq_t x = 1;          /* current x to test */

    for (;; ++x) {
        /* t = x^2 + p;  must be even so that y^2 = t/2 is integer. */
        seq_t t = x * x + (seq_t)p;
        if ((t & 1) != 0) continue;          /* odd -> cannot be 2*y^2 */

        seq_t y2 = t / 2;                     /* y^2 candidate */
        seq_t y  = floor_sqrt(y2);            /* integer sqrt */
        if (y * y == y2) {
            ++found;
            if (found == 2) return x;         /* a(n) is the second solution */
        }
    }
    /* should never reach here */
    abort();
}

/* -------------------------------------------------------------------------- */
/* Public sequence function:  a(n)                                           */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour below offset is undefined */

    /* build the list of primes until we have at least n primes   */
    while (prime_len < (size_t)n) {
        last_gen_prime = next_prime(last_gen_prime + 1);
        if ((last_gen_prime & 7) == 1 || (last_gen_prime & 7) == 7) {
            ensure_prime_capacity(prime_len + 1);
            prime_list[prime_len++] = last_gen_prime;
        }
    }

    /* fetch or compute the n‑th term */
    ensure_value_capacity((size_t)n);
    if (value_cache[n - 1] == (seq_t)0) {
        uint64_t p = prime_list[n - 1];
        value_cache[n - 1] = find_second_solution(p);
    }
    return value_cache[n - 1];
}

/*
 * A317710  Uniform tree numbers. Matula-Goebel numbers of uniform rooted
 *          trees.
 * https://oeis.org/A317710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n‑th uniform tree number by scanning all
 * positive integers, testing each for the uniform tree property,
 * and memoising the results.  The definition of a uniform tree
 * number is:
 *
 *   n is uniform if n = 1 or n = s^k where s is squarefree,
 *   all exponents are equal to k, and for each prime factor p of s
 *   the ordinal of p among primes (prime index) is itself a uniform
 *   tree number.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper functions from the utils directory */
#include "../utils/build_primes.h"
#include "../utils/build_spf.h"

static idx_t current_limit = 0;           /* size of arrays currently in use   */
static idx_t *spf = NULL;                 /* smallest prime factor table        */
static idx_t *prime_list = NULL;          /* list of primes up to current_limit*/
static size_t prime_count = 0;            /* number of primes                   */
static idx_t *prime_index = NULL;         /* prime -> ordinal (1‑based)         */

static int8_t *uniform_flag = NULL;       /* -1 unknown, 0 false, 1 true      */

static seq_t *uniform_vals = NULL;        /* collected uniform numbers          */
static size_t uniform_len = 0, uniform_cap = 0;

static idx_t test_cur = 1;                /* current integer being tested      */

/* ------------------------------------------------------------------ */
/* Ensure all tables are large enough for at least 'need' indices.    */
static void extend_limit(idx_t need)
{
    if (need <= current_limit)
        return;

    /* Choose a new size: double until large enough. */
    idx_t new_limit = current_limit ? current_limit << 1 : 128;
    while (new_limit < need)
        new_limit <<= 1;

    /* Free old tables. */
    free(spf);
    free(prime_list);
    free(prime_index);

    /* Build new tables. */
    build_spf(new_limit, &spf);
    build_primes(new_limit, &prime_list, &prime_count);

    /* Allocate prime_index array. */
    prime_index = (idx_t *)malloc((new_limit + 1) * sizeof(idx_t));
    memset(prime_index, 0, (new_limit + 1) * sizeof(idx_t));
    for (size_t i = 0; i < prime_count; ++i)
        prime_index[prime_list[i]] = (idx_t)(i + 1);    /* 1‑based index */

    /* Grow uniform_flag table. */
    if (uniform_flag) {
        int8_t *new_flag = (int8_t *)malloc((new_limit + 1) * sizeof(int8_t));
        memcpy(new_flag, uniform_flag, (current_limit + 1) * sizeof(int8_t));
        /* Initialise new entries to -1 */
        memset(new_flag + current_limit + 1, 0xFF, (new_limit - current_limit) * sizeof(int8_t));
        free(uniform_flag);
        uniform_flag = new_flag;
    } else {
        uniform_flag = (int8_t *)malloc((new_limit + 1) * sizeof(int8_t));
        memset(uniform_flag, 0xFF, (new_limit + 1) * sizeof(int8_t));  /* all -1 */
    }

    current_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Test whether x is a uniform tree number.  Memoised results are    */
/* stored in uniform_flag.                                            */
static int8_t is_uniform(idx_t x)
{
    if (x <= current_limit && uniform_flag[x] != -1)
        return uniform_flag[x];

    extend_limit(x);                     /* ensure tables are large enough */

    if (x == 1) {
        uniform_flag[1] = 1;
        return 1;
    }

    /* Factorise x. All exponents must be equal. */
    idx_t t = x;
    idx_t first_exp = 0;
    bool equal_exps = true;

    while (t > 1) {
        idx_t p = spf[t];
        if (p == 0)                      /* defensive, should not happen */
            p = t;

        idx_t exp = 0;
        while (t % p == 0) {
            t /= p;
            exp++;
        }

        if (first_exp == 0)
            first_exp = exp;
        else if (exp != first_exp) {
            equal_exps = false;
            break;
        }

        /* Check the prime index of p is uniform. */
        idx_t m = prime_index[p];
        if (!is_uniform(m)) {
            equal_exps = false;
            break;
        }
    }

    uniform_flag[x] = equal_exps ? 1 : 0;
    return uniform_flag[x];
}

/* ------------------------------------------------------------------ */
/* Append a uniform number to the collected list.                     */
static void append_uniform(idx_t val)
{
    if (uniform_len == uniform_cap) {
        size_t newcap = uniform_cap ? uniform_cap << 1 : 16;
        uniform_vals = (seq_t *)realloc(uniform_vals, newcap * sizeof(seq_t));
        uniform_cap = newcap;
    }
    uniform_vals[uniform_len++] = (seq_t)val;
}

/* ------------------------------------------------------------------ */
/* Exported function: returns the n‑th uniform tree number.           */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for the offset 1, but handle gracefully */
        return 0;

    while (uniform_len < n) {
        if (is_uniform(test_cur))
            append_uniform(test_cur);
        test_cur++;
    }
    return uniform_vals[n - 1];
}

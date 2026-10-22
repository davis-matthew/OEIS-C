/*
 * A308833  Numbers r such that the r-th tetrahedral number A000292(r) divides
 *          r!.
 * https://oeis.org/A308833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer r such that the r‑th tetrahedral number
 *            T_r = r(r+1)(r+2)/6 divides r! . The implementation tests this
 *            divisibility by comparing prime exponents of r! (Legendre) with
 *            those of T_r
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility for generating primes up to a limit */
#include "../utils/build_primes.h"

/* ----------------- Helper data ------------------------------------- */

static idx_t  *prime_list = NULL;   /* array of primes */
static size_t prime_cnt   = 0;
static idx_t  prime_limit = 0;

/* Ensure that primes up to 'limit' are available */
static void
ensure_primes(idx_t limit)
{
    if (prime_limit < limit) {
        build_primes(limit, &prime_list, &prime_cnt);
        prime_limit = limit;
    }
}

/* ------------------------------------------------------------------ */

/* Compute how many times prime 'p' occurs in n! (Legendre's formula) */
static unsigned int
legendre_exp(idx_t n, idx_t p)
{
    unsigned int e = 0;
    while (n) {
        n /= p;
        e += (unsigned int)n;
    }
    return e;
}

/* Add exponent for a prime to the local arrays, accumulating if it
   already exists.  */
static void
add_factor(idx_t p, unsigned int exp,
           idx_t *primes, unsigned int *exp_arr, size_t *cnt)
{
    for (size_t i = 0; i < *cnt; i++) {
        if (primes[i] == p) {
            exp_arr[i] += exp;
            return;
        }
    }
    primes[*cnt]   = p;
    exp_arr[*cnt]  = exp;
    (*cnt)++;
}

/* Factorize 'x' and add its prime decomposition to the arrays */
static void
factor_and_add(idx_t x,
               idx_t *primes, unsigned int *exp_arr, size_t *cnt)
{
    if (x <= 1) return;
    for (size_t i = 0; i < prime_cnt; i++) {
        idx_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) {
            unsigned int c = 0;
            while (x % p == 0) {
                x /= p;
                c++;
            }
            add_factor(p, c, primes, exp_arr, cnt);
        }
    }
    if (x > 1) add_factor(x, 1, primes, exp_arr, cnt);
}

/* Check whether r satisfies the divisibility condition. */
static bool
is_term(idx_t r)
{
    /* At most 32 distinct primes in r, r+1, r+2 */
    idx_t  primes[32];
    unsigned int exp_T[32];
    size_t   cnt = 0;

    /* Factor r, r+1, r+2 into the exponent arrays */
    factor_and_add(r,   primes, exp_T, &cnt);
    factor_and_add(r+1, primes, exp_T, &cnt);
    factor_and_add(r+2, primes, exp_T, &cnt);

    /* Subtract the factor 6 = 2*3 once */
    for (size_t i = 0; i < cnt; i++) {
        if (primes[i] == 2) exp_T[i]--;
        else if (primes[i] == 3) exp_T[i]--;
    }

    /* Verify exponent condition for each prime */
    for (size_t i = 0; i < cnt; i++) {
        idx_t p = primes[i];
        unsigned int need = exp_T[i];
        if (need == 0) continue;
        unsigned int have = legendre_exp(r, p);
        if (have < need) return false;
    }
    return true;
}

/* ----------------- Main sequence function --------------------------- */

seq_t a(idx_t n)
{
    /* static cache of terms */
    static seq_t *terms = NULL;
    static size_t cap   = 0;
    static size_t count = 0;
    static idx_t next_r = 2;   /* next r to test, after 1 */

    if (n == 0) return (seq_t)0;   /* offset=1, but safe guard */

    /* Initialize primes up to a safe limit (sufficient for n up to 10^5) */
    ensure_primes(200000);

    /* Initialise cache with the first term 1 */
    if (count == 0) {
        cap   = 32;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) exit(1);
        terms[0] = (seq_t)1;
        count   = 1;
        next_r  = 2;
    }

    /* Generate terms until we have at least n of them */
    while (count < n) {
        idx_t r = next_r;

        if (is_term(r)) {
            if (count == cap) {           /* grow array */
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
                cap   = new_cap;
            }
            terms[count++] = (seq_t)r;
        }
        next_r++;
    }

    return terms[n - 1];
}

/*
 * A344350  a(n) = Sum_{k=1..n} mu(n*k-k-1)^2, where mu is the Möbius
 *          function.
 * https://oeis.org/A344350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(k*(n-1)+1)^2 (i.e. count of square‑free
 *            numbers along the main antidiagonal of an n × n row‑major array
 *            of 1..n²)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*  simple prime sieve – generates all primes ≤ limit                     */
static void sieve_prime_list(idx_t limit, idx_t **out, size_t *cnt)
{
    if (limit < 2) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    /* Allocate a boolean marker array */
    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp)
        return; /* on allocation failure leave at NULL size 0 */

    size_t estimated = limit / 10; /* rough estimation of count */
    idx_t *pr = (idx_t *)malloc(estimated * sizeof(idx_t));
    if (!pr) {
        free(is_comp);
        return;
    }
    size_t pc = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_comp[(size_t)p]) {
            /* p is prime */
            if (pc == estimated) {
                estimated = (estimated == 0) ? 8 : estimated * 2;
                idx_t *tmp = (idx_t *)realloc(pr, estimated * sizeof(idx_t));
                if (!tmp) {
                    free(pr);
                    free(is_comp);
                    return;
                }
                pr = tmp;
            }
            pr[pc++] = p;
            if (p * p <= limit)
                for (idx_t q = p * p; q <= limit; q += p)
                    is_comp[(size_t)q] = true;
        }
    }
    free(is_comp);
    idx_t *tmp = (idx_t *)realloc(pr, pc * sizeof(idx_t));
    if (!tmp) {
        /* if realloc fails just keep the old block – it has enough space */
        *out = pr;
    } else
        *out = tmp;
    *cnt = pc;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  test whether a number is square‑free using the prime list          */
static inline bool is_squarefree(idx_t x, idx_t *primes, size_t prime_cnt)
{
    idx_t t = x;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > t)
            break;
        if (t % p == 0) {
            t /= p;
            if (t % p == 0)
                return false; /* p² divides x */
            while (t % p == 0)
                t /= p; /* remove higher powers of p */
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  main sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but we return 0 for safety */

    /* keep a static prime list up to the largest n requested */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (n > prime_limit) {
        /* build new prime list up to n */
        if (primes) {
            free(primes);
            primes = NULL;
        }
        sieve_prime_list(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    seq_t total = 0;
    idx_t n_minus_1 = n - 1;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t val = k * n_minus_1 + 1;
        if (is_squarefree(val, primes, prime_cnt))
            ++total;
    }
    return total;
}

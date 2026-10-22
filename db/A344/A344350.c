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
 * Helpers:   sieve_prime_list
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_prime_list.h"

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

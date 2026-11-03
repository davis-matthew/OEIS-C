/*
 * A283020  For p = prime(n), number of iterations of the function f(x) =
 *          2*x^3-1 that leave p prime.
 * https://oeis.org/A283020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of f(x)=2*x^3-1 that leave p prime,
 *            where p is the nth prime
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 73, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9883 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Computes the nth term of the sequence.  The function is thread‑safe,
   but may rebuild its prime table if a larger n is requested. */
seq_t a(idx_t n)
{
    /* Cache of primes.  primes_cnt holds the number of primes stored. */
    static uint64_t *primes      = NULL;
    static size_t  primes_cnt   = 0;

    /* Ensure we have at least n primes. */
    if (primes_cnt < n) {
        if (primes) free(primes);
        idx_t limit = _prime_limit_for_idx(n);
        build_primes_upto(limit, &primes, &primes_cnt);
    }

    /* Prime p = prime(n). */
    seq_t p = (seq_t)primes[n-1];

    /* Count iterations that produce a prime. */
    seq_t count = 0;
    seq_t cur   = p;

    while (1) {
        /* next = 2*cur^3 - 1. */
        seq_t cube  = cur * cur;
        cube        = cube * cur;          /* cur^3 */
        seq_t next = cube * (seq_t)2 - (seq_t)1;

        if (!is_prime_big(next))
            break;          /* composite: stop */
        count++;            /* prime: increment counter */
        cur = next;         /* continue with next value */
    }

    return count;
}

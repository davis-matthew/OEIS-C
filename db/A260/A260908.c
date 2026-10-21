/*
 * A260908  Numerator of 1/det(M) where M is the n X n matrix with M[i,j] =
 *          1/gcd(i,j).
 * https://oeis.org/A260908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Numerator of 1/det(M) where M[i,j] = 1/gcd(i,j)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  From the determinant factorization:                        */
/*    det(M) = ∏_{k=1}^{n} S(k)/k  with  S(k)=∏_{p|k} (1‑p).   */
/*  Hence a(n) = Numerator of ( n!  / ∏_{p≤n} (1‑p)^{⌊n/p⌋} ).  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/seq_pow.h"       /* provides seq_pow(seq_t, idx_t) */

static idx_t   max_limit   = 0;
static idx_t *primes      = NULL;
static size_t  primes_cnt = 0;
static size_t  primes_cap = 0;
static idx_t *spf         = NULL;
static size_t  spf_cap    = 0;

/* Build linear sieve up to 'limit', storing primes and smallest prime factor. */
static void build_sieve(idx_t limit) {
    if (limit <= max_limit) return;
    max_limit = limit;
    /* resize spf array */
    spf_cap = limit + 1;
    spf = (idx_t *)realloc(spf, spf_cap * sizeof(idx_t));
    memset(spf, 0, spf_cap * sizeof(idx_t));
    /* reset primes list */
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap == 0 ? 16 : primes_cap << 1;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || i * p > limit) break;
            spf[i * p] = p;
        }
    }
}

/* Return the sequence value a(n). */
seq_t a(idx_t n) {
    if (n < 1) return 0;                /* offset is 1; undefined for 0 */
    build_sieve(n);

    /* sign of a(n) = (-1)^{∑_{p≤n} ⌊n/p⌋} */
    idx_t sum_floor = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i)
        sum_floor += n / primes[i];
    const int negative = (sum_floor & 1) != 0;

    /* d_exp[q] holds exponent of prime q in denominator D = ∏_{p≤n} (p-1)^{⌊n/p⌋}  */
    idx_t *d_exp = (idx_t *)calloc(max_limit + 1, sizeof(idx_t));
    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i) {
        idx_t p = primes[i];
        idx_t e  = n / p;
        idx_t x  = p - 1;
        while (x > 1) {
            idx_t q = spf[x];
            idx_t cnt = 0;
            while (x % q == 0) {
                x /= q;
                ++cnt;
            }
            d_exp[q] += cnt * e;
        }
    }

    /* Compute exponent of each prime q in n! and in a(n) after cancellation */
    seq_t result = 1;
    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i) {
        idx_t q = primes[i];
        /* exponent of q in n!  */
        idx_t v = 0, tmp = n;
        while (tmp) {
            tmp /= q;
            v += tmp;
        }
        /* exponent of q in denominator D  */
        idx_t d = d_exp[q];
        if (v > d) {                     /* remaining exponent in numerator */
            idx_t exp = v - d;
            result *= seq_pow((seq_t)q, exp);
        }
    }
    free(d_exp);

    if (negative) result = -result;
    return result;
}

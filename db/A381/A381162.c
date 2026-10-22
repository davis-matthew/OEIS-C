/*
 * A381162  a(n) = (8*n)!/((n!)^4*(4*n)!).
 * https://oeis.org/A381162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n)! / ((n!)^4 * (4*n)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Utilities provided by the build system */
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Prime exponent in m! (Legendre's formula) */
static idx_t legendre(idx_t m, idx_t p) {
    idx_t exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

/* Compute a(n) */
seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* ----------------------------------------------------------------
     * Cache of primes up to the largest required limit.
     * -------------------------------------------------------------- */
    static idx_t max_cached = 0;
    static uint64_t *primes   = NULL;
    static size_t  prime_cnt = 0;

    idx_t limit = 8 * n;            /* we need primes ≤ 8n */

    if (limit > max_cached) {
        /* Allocate memory for sieve */
        size_t sz = (size_t)limit + 1;
        uint8_t *is_comp = (uint8_t *)calloc(sz, sizeof(uint8_t));
        if (!is_comp) abort();

        /* Count primes */
        size_t count = 0;
        for (size_t i = 2; i < sz; ++i) {
            if (!is_comp[i]) {
                ++count;
                if (i * i < sz) {
                    for (size_t j = i * i; j < sz; j += i)
                        is_comp[j] = 1;
                }
            }
        }

        /* Allocate array and fill primes */
        uint64_t *new_primes = (uint64_t *)calloc(count, sizeof(uint64_t));
        if (!new_primes) {
            free(is_comp);
            abort();
        }

        size_t idx = 0;
        for (size_t i = 2; i < sz; ++i) {
            if (!is_comp[i])
                new_primes[idx++] = (uint64_t)i;
        }

        free(is_comp);
        free(primes);
        primes     = new_primes;
        prime_cnt  = count;
        max_cached = limit;
    }

    /* ----------------------------------------------------------------
     * Compute exponent of each prime and accumulate the result.
     * -------------------------------------------------------------- */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p > (seq_t)limit) break;   /* should never happen */

        idx_t exp8  = legendre(8 * n, (idx_t)p);
        idx_t exp4  = legendre(4 * n, (idx_t)p);
        idx_t expn  = legendre(n,     (idx_t)p);
        idx_t exp   = exp8 - 4 * expn - exp4;   /* exponent in a(n) */

        if (exp > (idx_t)0) {
            seq_t pp = seq_pow((seq_t)p, exp);
            result   = seq_mul(result, pp);
        }
    }

    return result;
}

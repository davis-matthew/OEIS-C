/*
 * A280735  Numerator of Product_{k=1..n-1} k^(2k-n-1).
 * https://oeis.org/A280735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Π_{k=1}^{n+1} k^{2k-n-3} (numerator of that fraction)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */

/* Smallest Prime Factor table (spf[i] == smallest prime factor of i; spf[0]=spf[1]=0) */
static idx_t *spf      = NULL;
static idx_t  spf_limit = 0;

static void init_spf_upto(idx_t limit) {
    if (limit <= spf_limit) return;

    spf = (idx_t*)realloc(spf, (limit + 1) * sizeof(idx_t));
    if (!spf) abort(); /* out of memory */

    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {                /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_limit = limit;
}

/* Exponent table for primes: expose[p] contains net exponent of prime p in the
   product Π k^{2k - n - 3}.  expo_size is the allocated size, which must be
   at least max_base+1. */
static long long *expos = NULL;
static idx_t    expos_size = 0;

/* Ensure the exponent table is large enough. */
static void ensure_expos_size(idx_t size) {
    if ((size + 1) > expos_size) {
        expos = (long long*)realloc(expos, (size + 1) * sizeof(long long));
        if (!expos) abort();
        expos_size = size;
    }
}

/* Main routine. */
seq_t a(idx_t n) {
    /* We need to consider k up to n+1. */
    idx_t max_base = n + 1;
    init_spf_upto(max_base);
    ensure_expos_size(max_base);

    /* Zero the exponent table. */
    memset(expos, 0, (max_base + 1) * sizeof(long long));

    /* Accumulate net prime exponents. */
    for (idx_t k = 1; k <= max_base; ++k) {
        long long coeff = (long long)2 * (long long)k - (long long)n - 3LL;
        if (coeff == 0) continue;

        idx_t tmp = k;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            expos[p] += coeff * cnt;
        }
    }

    /* Build the numerator from primes with positive exponent. */
    seq_t result = (seq_t)1;
    for (idx_t p = 2; p <= max_base; ++p) {
        if (spf[p] == p && expos[p] > 0) {
            seq_t pw = big_pow((seq_t)p, (seq_t)expos[p]);
            result = seq_mul(result, pw);
        }
    }
    return result;
}

/*
 * A349375  Dirichlet convolution of Kimberling's paraphrase, A003602, with
 *          Liouville's lambda.
 * https://oeis.org/A349375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003602(n/d) * A008836(d)
 * Range:     u8 n <= 437, u16 n <= 8732, u32 n <= 8732, u64 n <= 8732,
 *            u128 n <= 8732, big any n
 * Helpers:   B_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/B_seq.h"

/* --------------------------------------------------------------------
   Smallest prime factor table (sieve).  Allocated lazily and expanded
   as needed.  All functions that use it are static helpers.
   -------------------------------------------------------------------- */
static idx_t *spf = NULL;
static size_t spf_size = 0;

static void ensure_spf(idx_t N) {
    if (N + 1 <= spf_size) return;      /* already ready */

    size_t new_size = (size_t)N + 1;
    idx_t *new_spf = (idx_t *)malloc(new_size * sizeof(idx_t));
    if (!new_spf) exit(1);

    for (size_t i = 0; i < new_size; ++i) new_spf[i] = (idx_t)i;

    idx_t limit = (idx_t)sqrt((double)(new_size - 1));
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == i) {
            for (idx_t j = i * i; j < new_size; j += i) {
                if (new_spf[j] == j) new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf      = new_spf;
    spf_size = new_size;
}

/* --------------------------------------------------------------------
   Recursive divisor enumeration.  Adds λ(d)·A003602(n/d) to the sum.
   -------------------------------------------------------------------- */
static void rec_divisors(int idx,
                        idx_t n, idx_t current_d,
                        int omega,
                        const idx_t *primes, const int *exps,
                        int count,
                        int64_t *sum)
{
    if (idx == count) {                     /* a divisor is fully built */
        idx_t m = n / current_d;
        seq_t b = B_seq(m);
        if ((omega & 1) == 0) *sum += (int64_t)b;
        else                 *sum -= (int64_t)b;
        return;
    }

    idx_t p      = primes[idx];
    int   emax   = exps[idx];
    idx_t d      = current_d;

    for (int e = 0; e <= emax; ++e) {
        rec_divisors(idx + 1, n, d, omega + e,
                     primes, exps, count, sum);
        if (e < emax) d *= p;
    }
}

/* --------------------------------------------------------------------
   Main function computing a(n) as requested.
   -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* safety for out‑of‑range */
    ensure_spf(n);

    /* factorisation of n */
    idx_t   tmp   = n;
    idx_t   primes[64];
    int     exps[64];
    int     cnt   = 0;

    while (tmp > 1) {
        idx_t p = spf[tmp];
        int   e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[cnt] = p;
        exps[cnt]   = e;
        ++cnt;
    }

    int64_t total = 0;
    rec_divisors(0, n, 1, 0, primes, exps, cnt, &total);
    return (seq_t)total;                     /* total is non‑negative   */
}

/*
 * A276306  Number of pairs of integers (k, m) with k < m < n such that (k, m,
 *          n) is an abc-triple.
 * https://oeis.org/A276306
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   build_spf, gcd, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A276306 – Number of pairs (k,m) with k < m < n,
    gcd(k,m)=1 and rad(k,m,n) < n,  for n ≥ 3.
    The function pre‑computes a smallest‑prime‑factor sieve up to the
    current maximum n, stores the rad(n) values, and then counts
    qualifying pairs using only integer arithmetic.                           */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"           /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/gcd_int.h"       /* int gcd_int(int a, int b) */
#include "../utils/build_spf.h"     /* void build_spf(idx_t limit, idx_t **spf_out) */

seq_t a(idx_t n)
{
    /* Skip indices before the offset – they are unspecified */
    if (n < 3) return (seq_t)0;

    /* Static storage for the sieve and rad array */
    static idx_t cache_limit = 0;
    static idx_t *spf = NULL;           /* smallest prime factor of each i */
    static uint64_t *rad_cache = NULL;  /* rad[i] as 64‑bit integer */

    /* If the current maximum n exceeds the pre‑computed range,
       reallocate and rebuild the data structures. */
    if (n > cache_limit) {
        /* free previous allocations */
        if (spf)      free(spf);
        if (rad_cache) free(rad_cache);

        /* Build the SPF array up to n (inclusive) */
        build_spf(n, &spf);                 /* spf[0..n] */

        /* Allocate rad array and compute rad[i] for all i ≤ n */
        rad_cache = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
        rad_cache[0] = 1;
        rad_cache[1] = 1;
        for (idx_t i = 2; i <= n; ++i) {
            idx_t p = spf[i];
            idx_t j = i / p;
            if (j % p == 0)            /* p already occurs in j */
                rad_cache[i] = rad_cache[j];
            else                       /* new distinct prime factor p */
                rad_cache[i] = rad_cache[j] * (uint64_t)p;
        }

        cache_limit = n;
    }

    /* Count all pairs (k,m) such that k < m < n,
       gcd(k,m)=1 and rad(k)·rad(m)·rad(n) < rad(gcd(k,n))·rad(gcd(m,n)). */
    seq_t count = (seq_t)0;
    uint64_t rad_n = rad_cache[n];

    for (idx_t k = 1; k * 2 < n; ++k) {     /* ensure k < m = n - k */
        idx_t m = n - k;

        /* gcd(k,m) == 1 is a necessary condition */
        if (gcd_int((int)k, (int)m) != 1)
            continue;

        uint64_t rad_k = rad_cache[k];
        uint64_t rad_m = rad_cache[m];
        uint64_t rad_gkn = rad_cache[gcd_int((int)k, (int)n)];
        uint64_t rad_gmn = rad_cache[gcd_int((int)m, (int)n)];

        /* rad(k,m,n) = rad_k * rad_m * rad_n / rad_gkn / rad_gmn  (all integers) */
        uint64_t rad_abc = rad_k * rad_m;
        rad_abc *= rad_n;
        rad_abc /= rad_gkn;
        rad_abc /= rad_gmn;

        if (rad_abc < n)
            count++;
    }

    return count;
}

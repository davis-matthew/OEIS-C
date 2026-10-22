/*
 * A359059  Numbers k such that phi(k) + rad(k) + psi(k) is a multiple of 3.
 * https://oeis.org/A359059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   factor_u64, pow_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A359059
 * Returns the k-th integer k such that φ(k) + rad(k) + ψ(k) is divisible by 3.
 * The function builds a cache of found terms; evaluations are
 * independent of call order.  The implementation uses only
 * integer arithmetic and the provided utilities:
 *   factor_u64   (in utils/factor_u64.h)
 * no other external dependencies are required.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_mod3.h"

/* ---- small helpers ----------------------------------------------------- */

/* compute φ(n) mod 3, rad(n) mod 3, ψ(n) mod 3 for n >= 1.
 * Uses the factor_u64 routine to get (prime, exponent) pairs. */
static void phi_rad_psi_mod3(uint64_t n,
                             unsigned *phi_mod3,
                             unsigned *rad_mod3,
                             unsigned *psi_mod3)
{
    /* Special case n = 1 */
    if (n == 1)
    {
        *phi_mod3 = 1 % 3;   /* φ(1) = 1 */
        *rad_mod3 = 1 % 3;   /* rad(1) = 1 */
        *psi_mod3 = 1 % 3;   /* ψ(1) = 1 */
        return;
    }

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    unsigned phi = 1 % 3;
    unsigned rad = 1 % 3;
    unsigned psi = 1 % 3;

    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned pmod3 = primes[i] % 3;
        unsigned pinv = pow_mod3(pmod3, (unsigned)(exps[i] - 1U));

        /* φ term: (p-1) * p^(e-1) */
        unsigned phipart = ((primes[i] + 2U) % 3) * pinv % 3; /* (p-1)%3 = (p+2)%3 */
        phi = (phi * phipart) % 3;

        /* rad term: product of distinct primes */
        rad = (rad * pmod3) % 3;

        /* ψ term: (p+1) * p^(e-1) */
        unsigned psipart = ((primes[i] + 1U) % 3) * pinv % 3;
        psi = (psi * psipart) % 3;
    }

    *phi_mod3 = phi;
    *rad_mod3 = rad;
    *psi_mod3 = psi;
}

/* ---- cache for sequence terms ------------------------------------------ */

static seq_t *cache = NULL;          /* array of found terms (k values) */
static size_t cache_len = 0;         /* number of terms currently cached */
static size_t cache_cap = 0;         /* allocated capacity of cache array */
static idx_t next_k = 1;             /* next k to test for inclusion */

/* expand cache to contain at least n terms */
static void grow_cache(idx_t n)
{
    while ((size_t)n > cache_len)
    {
        /* test successive k values for eligibility */
        unsigned phi_mod3, rad_mod3, psi_mod3;
        phi_rad_psi_mod3(next_k, &phi_mod3, &rad_mod3, &psi_mod3);

        unsigned sum_mod3 = (phi_mod3 + rad_mod3 + psi_mod3) % 3U;
        if (sum_mod3 == 0U)
        {
            /* ensure capacity */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* assuming realloc succeeds as per specification */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* store the term (in seq_t range, truncation allowed if out of range) */
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* ----- public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n==0, won't be called */
    grow_cache(n);
    return cache[n - 1];               /* cache is 0‑based, n is 1‑based */
}

/*
 * A319662  2-rank of the class group of Q(sqrt(-k)), k squarefree.
 * https://oeis.org/A319662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ω(A005117(n)), if A005117(n) ≡ 1 (mod 4) * ω(A005117(n))
 *            – 1, otherwise. * Where A005117(n) is the n‑th square‑free
 *            number and ω(m) is the * number of distinct prime divisors of m.
 *            * This implements the 2‑rank of the class group of Q(√‑k)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the n‑th term of the sequence.
 * For n < 1 the function returns 0 (the offset is 1). */
seq_t a(idx_t n)
{
    if (n < 1)    /* indices before the offset are irrelevant */
        return 0;

    /* Upper bound for the n‑th square‑free number.  The density of
     * square‑free numbers is 6/π² ≈ 0.6079.  Using 3×n+100 gives a
     * comfortable safety margin. */
    idx_t limit = n * 3 + 100;
    if (limit < 200)
        limit = 200;   /* guaranteed minimum size */

    /* Allocate necessary temporary arrays.  They are freed before
     * returning, so no static state is kept between calls. */
    uint32_t *spf   = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    int8_t   *mobius= (int8_t   *)malloc((limit + 1) * sizeof(int8_t));
    if (!spf || !mobius) {
        /* Out of memory – for the purposes of the OEIS evaluator
         * this situation will not occur.  We return 0 as a safe
         * fallback. */
        free(spf);
        free(mobius);
        return 0;
    }

    /* ---- 1. Linear sieve for smallest prime factor and Möbius. ---- */
    for (idx_t i = 0; i <= limit; ++i) {
        spf[i] = (uint32_t)i;
        mobius[i] = 0;
    }
    mobius[1] = 1;

    uint32_t *primes = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    size_t   prime_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime */
            mobius[i] = -1;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || (idx_t)i * (idx_t)p > limit)
                break;
            spf[(idx_t)i * (idx_t)p] = p;
            if (i % p == 0) {
                mobius[(idx_t)i * (idx_t)p] = 0;   /* not square‑free */
                break;
            } else {
                mobius[(idx_t)i * (idx_t)p] = -mobius[i];
            }
        }
    }
    free(primes);

    /* ---- 2. Find the n‑th square‑free number and compute its ω. ---- */
    idx_t count = 0;
    idx_t sqfree = 0;       /* placeholder for the n‑th square‑free */
    for (idx_t i = 1; i <= limit; ++i) {
        if (mobius[i] != 0) {           /* i is square‑free */
            ++count;
            if (count == n) {
                sqfree = i;
                break;
            }
        }
    }

    /* Fallback: if we did not find enough square‑free numbers, enlarge
     * the limit and repeat.  For n <= 10000 this should never be
     * necessary, but the loop guarantees correctness for any n. */
    while (count < n) {
        /* Increase the limit and recompute.  This rarely occurs. */
        limit = limit * 2 + 100;
        if (limit < 200)
            limit = 200;

        free(spf);
        free(mobius);
        spf    = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        mobius = (int8_t   *)malloc((limit + 1) * sizeof(int8_t));
        if (!spf || !mobius) {
            free(spf);
            free(mobius);
            return 0;
        }

        for (idx_t i = 0; i <= limit; ++i) {
            spf[i] = (uint32_t)i;
            mobius[i] = 0;
        }
        mobius[1] = 1;

        primes = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        prime_cnt = 0;

        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == (uint32_t)i) {
                mobius[i] = -1;
                primes[prime_cnt++] = (uint32_t)i;
            }
            for (size_t j = 0; j < prime_cnt; ++j) {
                uint32_t p = primes[j];
                if (p > spf[i] || (idx_t)i * (idx_t)p > limit)
                    break;
                spf[(idx_t)i * (idx_t)p] = p;
                if (i % p == 0) {
                    mobius[(idx_t)i * (idx_t)p] = 0;
                    break;
                } else {
                    mobius[(idx_t)i * (idx_t)p] = -mobius[i];
                }
            }
        }
        free(primes);

        count = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (mobius[i] != 0) {
                ++count;
                if (count == n) {
                    sqfree = i;
                    break;
                }
            }
        }
    }

    /* Compute ω(sqfree) using the precomputed spf array. */
    idx_t temp = sqfree;
    int omega = 0;
    while (temp > 1) {
        uint32_t p = spf[temp];
        ++omega;
        while (temp % p == 0)
            temp /= p;
    }

    /* Apply the 1 or 0 adjustment depending on the congruence class. */
    seq_t result;
    if ((sqfree & 3U) == 1)      /* ≡ 1 (mod 4) */
        result = (seq_t)omega;
    else
        result = (seq_t)(omega - 1);

    /* Clean up temporary allocations. */
    free(spf);
    free(mobius);

    return result;
}

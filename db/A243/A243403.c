/*
 * A243403  Number of primes p < n such that p*(n-p) is a primitive root
 *          modulo prime(n).
 * https://oeis.org/A243403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of primes p < n such that p*(n‑p) is a primitive
 *            root modulo prime(n). The implementation follows the definition
 *            exactly, using only the provided helper routines and basic
 *            arithmetic
 * Range:     u8 n <= 54, u16 n <= 6757, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9234 (2026-11-02)
 * Helpers:   build_primes_upto, factor_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/powmod.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* -------------------------------------------------------------------------- */
/*  Main sequence function (external linkage)                                 */
seq_t a(idx_t n)
{
    /* OEIS offset is 1, but return 0 for indices < 1 to keep function safe */
    if (n < 1) return 0;

    /* --- 1.  Get the n‑th prime, prime(n) --------------------------------  */
    uint32_t prime_n32 = get_prime(n);             /* fits in uint32_t          */
    uint64_t prime_n    = (uint64_t)prime_n32;     /* promote for arithmetic     */
    seq_t prime_seq     = (seq_t)prime_n;          /* convert to seq_t           */
    uint64_t phi_val    = prime_n - 1;             /* φ(prime(n))                */

    /* --- 2.  Factor φ(prime(n)) to get its distinct prime factors --------  */
    uint64_t prime_facs[32];
    uint64_t exps[32];
    size_t   fcnt = 0;
    factor_u64(phi_val, prime_facs, exps, &fcnt);   /* prime_facs holds factor list */

    /* --- 3.  Generate all primes < n ------------------------------------  */
    uint64_t *primes_less = NULL;
    size_t    cnt_less    = 0;
    if (n > 1)
    {
        size_t limit = (size_t)(n - 1);
        build_primes_upto(limit, &primes_less, &cnt_less);
    }

    /* --- 4.  Count primes p < n where p*(n-p) is a primitive root ------- */
    seq_t count = 0;
    for (size_t i = 0; i < cnt_less; ++i)
    {
        uint64_t p = primes_less[i];                         /* prime p < n  */
        /* Compute a_mod = p * (n - p) mod prime(n) */
        seq_t a_mod = ((seq_t)p * (seq_t)(n - p)) % prime_seq;
        if (a_mod == 0) continue;                            /* cannot be root */

        bool ok = true;
        for (size_t f = 0; f < fcnt; ++f)
        {
            uint64_t q      = prime_facs[f];
            uint64_t exp_val = phi_val / q;                /* exponent for test */
            if (powmod(a_mod, exp_val, prime_seq) == 1)
            {
                ok = false;
                break;
            }
        }
        if (ok) ++count;
    }

    /* Free the array allocated by build_primes_upto */
    if (primes_less) free(primes_less);

    return count;
}

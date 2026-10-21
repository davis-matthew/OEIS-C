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
 * Helpers:   build_primes_upto, factor_u64, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k);          */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit,
                                                   uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/factor_u64.h"          /* void factor_u64(uint64_t n,
                                                   uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/mod_pow.h"            /* seq_t mod_pow(seq_t base, uint64_t exp,
                                                   seq_t mod); */

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
            if (mod_pow(a_mod, exp_val, prime_seq) == 1)
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

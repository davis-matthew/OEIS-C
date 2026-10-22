/*
 * A368650  a(n) = (6*n + 1)!*(9*n + 1)!/((2*n)!*(3*n)!*((5*n + 1)!)^2).
 * https://oeis.org/A368650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n + 1)! * (9*n + 1)! / ((2*n)! * (3*n)! * ((5*n +
 *            1)!)^2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   build_primes, prime_exp_in_fact, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses prime‑factorisation with Legendre’s
   formula.  No caching of the sequence values is required – the
   function computes a(n) directly.  A cache of the prime table is
   maintained to avoid rebuilding it for larger arguments.  The code
   is valid C11 and C++17 and works for seq_t being a builtin integer
   type or an arbitrary-precision class.  */

#include <stdlib.h>          /* free, abort */
#include <stddef.h>          /* size_t */
#include <stdbool.h>         /* bool */

/* Utility headers – the build system provides these implementations.  */
#include "../utils/build_primes.h"   /* build_primes() */
#include "../utils/seq_mul.h"        /* seq_mul() */
#include "../utils/seq_pow.h"        /* seq_pow() */
#include "../utils/prime_exp_in_fact.h"

/* ------------------------------------------------------------------ */
/*  Helper: exponent of prime p in n!  (Legendre’s formula)           */

/* ------------------------------------------------------------------ */
/*  Helper: compute a single term a(n)                                 */
/* ------------------------------------------------------------------ */
static seq_t term_for(idx_t n)
{
    /* Prime table that covers up to 9*n+1. */
    static idx_t *primes = NULL;       /* array of primes */
    static size_t prime_cnt = 0;       /* number of primes stored */
    static idx_t prime_limit = 0;      /* largest number for which primes were generated */

    idx_t need = 9 * n + 1U;  /* the largest factorial argument we must consider */

    if (need > prime_limit) {
        /* Rebuild the prime table for the new limit.  */
        if (primes) {
            free(primes);          /* free old memory */
        }
        build_primes(need, &primes, &prime_cnt);
        prime_limit = need;
    }

    /* Arguments of factorials. */
    idx_t six  = 6 * n + 1U;
    idx_t nine = 9 * n + 1U;
    idx_t two  = 2 * n;
    idx_t three = 3 * n;
    idx_t five_f = 5 * n + 1U;

    seq_t result = (seq_t)1;

    /* Multiply over all primes up to 9*n+1. */
    for (size_t i = 0; i < prime_cnt && primes[i] <= nine; ++i) {
        idx_t p = primes[i];
        idx_t e = prime_exp_in_fact(six,  p) + prime_exp_in_fact(nine, p)
                - prime_exp_in_fact(two,   p) - prime_exp_in_fact(three, p)
                - 2 * prime_exp_in_fact(five_f, p);

        if (e > 0) {
            seq_t pe = seq_pow((seq_t)p, e);
            result = seq_mul(result, pe);
        }
    }

    return result;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the build framework                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Compute and return a(n).  No caching of values is necessary.
       The function is tolerant to arbitrary call order. */
    return term_for(n);
}

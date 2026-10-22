/*
 * A341340  a(n) is the least k such that the product of n+1 consecutive
 *          primes starting with the k-th prime is greater than or equal to
 *          the product of the following n primes.
 * https://oeis.org/A341340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that ∏_{i=0}^n p_{k+i} ≥ ∏_{i=1}^n p_{k+n+i}
 * Range:     u8 n <= 42, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (p_m denotes the m‑th prime, p_1 = 2). This implementation uses
   a incremental prime generator and a running array of prime
   logarithms to compare products without overflow. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>   /* for size_t */

/* ------------------------------------------------------------------ */
/* Static data for prime generation                                       */
static uint32_t *primes = NULL;   /* array of primes (p_1, p_2, ...)          */
static size_t    primes_count = 0;/* number of primes stored                  */
static size_t    primes_cap = 0;   /* array allocation capacity                */

static double *logp = NULL;      /* cumulative log sums: logp[i] = Σ_{j=1}^i log(p_j) */
static size_t  logp_len = 0;     /* number of entries in logp (should be primes_count+1)*/
static size_t  logp_cap = 0;     /* allocation capacity for logp                    */

/* ------------------------------------------------------------------ */
/* Ensure that we have the first 'needed' primes and the corresponding
   cumulative logarithms. Return immediately if already satisfied.   */
static void ensure_primes(size_t needed)
{
    if (primes_count >= needed)
        return;

    /* Initialise prime array if needed. */
    if (!primes) {
        primes_cap = 16;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes)
            return; /* allocation failure – caller will fail gracefully */
    }

    /* Initialise logp array if needed. */
    if (!logp) {
        logp_cap = 16;
        logp = (double *)malloc(logp_cap * sizeof(double));
        if (!logp)
            return;
        logp[0] = 0.0;     /* logp[0] = 0 by definition */
        logp_len = 1;
    }

    while (primes_count < needed) {
        uint32_t candidate = (primes_count == 0) ? 2 : primes[primes_count - 1] + 1;
        /* Find the next prime by trial division using the primes already found. */
        bool is_prime = true;
        for (;;) {
            uint32_t lim = (uint32_t)floor(sqrt((double)candidate));
            is_prime = true;
            for (size_t i = 0; i < primes_count && primes[i] <= lim; ++i) {
                if (candidate % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;
            ++candidate;
        }

        /* Grow the prime array if necessary. */
        if (primes_count == primes_cap) {
            primes_cap *= 2;
            uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            if (!tmp) {  /* allocation failure */
                return;
            }
            primes = tmp;
        }
        primes[primes_count++] = candidate;

        /* Grow the logp array if necessary. */
        if (logp_len == logp_cap) {
            logp_cap *= 2;
            double *tmp = (double *)realloc(logp, logp_cap * sizeof(double));
            if (!tmp) {
                return;
            }
            logp = tmp;
        }
        double last = logp[logp_len - 1];
        logp[logp_len++] = last + log((double)candidate);
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                                 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* The offset is at 1; below it is undefined. */

    size_t sn = (size_t)n;

    for (idx_t k = 1; ; ++k) {
        size_t need = (size_t)k + 2 * sn;         /* we need primes up to index k+2n */

        ensure_primes(need);                     /* guarantees logp[need] exists */

        size_t num_end = (size_t)k + sn;          /* last index of the 'low' product */

        double lhs = logp[num_end] - logp[(size_t)k - 1];
        double rhs = logp[need]     - logp[num_end];

        if (lhs >= rhs)                      /* condition satisfied */
            return (seq_t)k;
    }
}

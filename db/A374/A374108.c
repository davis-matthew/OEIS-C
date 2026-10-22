/*
 * A374108  Numbers k such that A113177(k) and A328845(k) are both even, where
 *          A113177 is fully additive with a(p) = Fibonacci(p) and A328845 is
 *          the first Fibonacci-based variant of the arithmetic derivative.
 * https://oeis.org/A374108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that both A113177(k) and A328845(k) are even. * *
 *            A113177 is fully additive with a(p) = Fibonacci(p). Its parity
 *            is * Σ_{p odd} (exp(p) mod 2) * (Fib(p) mod 2) (mod 2) * where
 *            Fib(p) is odd iff p % 3 != 0. * * A328845 is the Fibonacci‐based
 *            arithmetic derivative. * If n = 2^e₂ · m...
 * Range:     u8 n <= 69, u16 n <= 9843, u32 n <= 9843, u64 n <= 9843,
 *            u128 n <= 9843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static idx_t *spf = NULL;      /* smallest prime factor array */
static idx_t spf_limit = 0;    /* current limit of the spf array */

/* Build or extend the spf array up to "limit" (inclusive). */
static void build_spf(idx_t limit)
{
    if (limit <= spf_limit) return;                 /* already built */

    /* Allocate new array (limit+1 because indices go 0..limit) */
    idx_t *new_spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!new_spf) return;                         /* out of memory → crash */

    /* Initialise with zeros, set indices 0 and 1 */
    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    new_spf[0] = 0;
    new_spf[1] = 1;

    /* Simple sieve for smallest prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!new_spf[i]) {                         /* i is prime */
            new_spf[i] = i;
            if (i <= limit / i) {                  /* avoid overflow */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (!new_spf[j]) new_spf[j] = i;
            }
        }
    }

    /* Replace old array, free memory if any. */
    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Return the nth term of A374108.  n ≥ 1. */
seq_t a(idx_t n)
{
    idx_t count = 0;     /* how many terms found so far */
    idx_t k = 1;         /* current integer being tested */

    for (;; ++k) {
        /* Ensure we can factor k.  Grow the spf array if necessary. */
        if (k > spf_limit) build_spf(k * 2 + 100);

        /* Factor k and compute required parities. */
        idx_t tmp = k;
        idx_t e2 = 0;                /* exponent of prime 2 */
        bool sum_even = true;        /* parity of Σ_{p odd} (exp mod2)*(Fib(p) mod2) */

        while (tmp > 1) {
            idx_t p = spf[tmp];
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (p == 2) {
                e2 = exp;
            } else {
                if ((exp & 1u) && ((p % 3) != 0))   /* Fib(p) odd? */
                    sum_even = !sum_even;           /* toggle parity */
            }
        }

        /* Check whether k satisfies the parity conditions. */
        bool good = false;
        if (e2 == 0) {
            /* k is odd */
            if (sum_even) good = true;
        } else if (e2 == 1) {
            /* k has a single factor of 2 → never good */
            good = false;
        } else { /* e2 ≥ 2 */
            bool parity_bit = !sum_even;          /* 0 if even, 1 if odd */
            if ( ((e2 & 1u) == (uint8_t)parity_bit) )
                good = true;
        }

        if (good) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}

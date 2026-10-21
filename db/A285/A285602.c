/*
 * A285602  a(0) = 1; for n>1, a(n) is the numerator of b(n) =
 *          Product_{i=1..n} (prime(i)^2 + 1)/(prime(i)^2 - 1).
 * https://oeis.org/A285602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   gcd, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285602: a(n) = numerator of Product_{i=1..n} (prime(i)^2+1)/(prime(i)^2-1)
 * In reduced form, a(0)=1.  The function uses cross‑gcd reduction to keep
 * intermediate numbers small.  It relies on a prime sieve for the first n
 * primes and the gcd utility. */
/* Implementation detail: for n=0 returns 1, otherwise computes with cross
 * reduction between the numerator and denominator. */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    uint32_t *pr = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &pr, &cnt);          /* pr[0..n-1] = first n primes */

    seq_t num = 1;
    seq_t den = 1;

    for (size_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t p2 = p * p;
        seq_t tn = p2 + 1;        /* numerator part (p^2+1) */
        seq_t td = p2 - 1;        /* denominator part (p^2-1) */

        seq_t g1 = gcd(tn, den);  /* reduce tn with current den */
        tn /= g1;
        den /= g1;

        seq_t g2 = gcd(td, num);  /* reduce td with current num */
        td /= g2;
        num /= g2;

        num *= tn;                /* multiply reduced parts */
        den *= td;
    }

    /* Final reduction to guarantee lowest terms (aggressive) */
    seq_t g = gcd(num, den);
    num /= g;

    free(pr);
    return num;
}

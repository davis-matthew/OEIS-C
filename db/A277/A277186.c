/*
 * A277186  Sum of primes within 2n-wide closed interval centered upon
 *          prime(n).
 * https://oeis.org/A277186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all primes p such that prime(n)‑n ≤ p ≤ prime(n)+n
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where prime(n) is the n‑th prime (offset 1).                           */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Helper: compute the n‑th prime using a simple sieve.                     */
static uint64_t nth_prime(idx_t n)
{
    /* Estimate an upper bound for the n‑th prime using Dusart.           */
    idx_t limit;
    if (n < 6) {
        limit = 15;            /* covers prime(6)=13 */
    } else {
        double dn = (double)n;
        double est = dn * (log(dn) + log(log(dn)));
        limit = (idx_t)(est + 10.0);   /* safety margin */
    }

    /* Ensure the sieve range is large enough for [prime‑n, prime+n].    */
    limit += n;

    /* Keep expanding the limit until it contains at least n primes.      */
    while (1) {
        size_t sz = (size_t)limit + 1;
        uint8_t *comp  = (uint8_t *)calloc(sz, 1);            /* 0 → prime, 1 → composite */
        uint64_t *prms = (uint64_t *)calloc(sz, sizeof(uint64_t));
        if (!comp || !prms) {
            free(comp); free(prms);
            return 0;        /* memory failure */
        }

        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!comp[i]) {
                prms[cnt++] = (uint64_t)i;
                if (i <= limit / i) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        comp[j] = 1;
                }
            }
        }

        if (cnt >= (size_t)n) {
            uint64_t pn = prms[n - 1];   /* 0‑based index → n‑th prime */
            free(comp); free(prms);
            return pn;
        }

        free(comp); free(prms);
        limit = limit * 2 + n;          /* increase and retry */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    /* Find prime(n).                                                      */
    uint64_t pn = nth_prime(n);
    if (pn == 0) return 0;              /* should never happen if memory OK */

    /* Interval bounds.                                                    */
    uint64_t low  = (pn > (uint64_t)n) ? (pn - n) : 1;
    uint64_t high = pn + n;

    /* Sieve again only up to high to sum primes inside the interval.    */
    idx_t limit = high;
    size_t sz = (size_t)limit + 1;
    uint8_t *comp  = (uint8_t *)calloc(sz, 1);
    uint64_t *prms = (uint64_t *)calloc(sz, sizeof(uint64_t));
    if (!comp || !prms) {
        free(comp); free(prms);
        return 0;                       /* memory failure */
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            prms[cnt++] = (uint64_t)i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    comp[j] = 1;
            }
        }
    }

    /* Sum primes within [low, high].                                      */
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k < cnt; ++k) {
        uint64_t p = prms[k];
        if (p < low)      continue;
        if (p > high)     break;
        sum += (seq_t)p;
    }

    free(comp); free(prms);
    return sum;
}

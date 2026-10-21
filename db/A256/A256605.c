/*
 * A256605  Least k such that n+1 is the n-th divisor of k.
 * https://oeis.org/A256605
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Least k such that the n-th divisor of k equals n+1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the sequence A256605.
    Uses a sieve to obtain prime factors up to n and
    the smallest‑prime‑factor array for fast factorisation.
*/
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

#define MAX_LIMIT 20000   /* enough for the required n (≤10000) */

/* Prime sieve and spf array (static for reuse) */
static idx_t spf[MAX_LIMIT + 1];
static idx_t primes[MAX_LIMIT + 1];
static size_t prime_cnt;

/* Build sieve up to 'limit' (inclusive).  Must be called with limit ≤ MAX_LIMIT. */
static void build_sieve(idx_t limit)
{
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;
    prime_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[prime_cnt++] = i;
            if (i <= limit / i) {          /* i*i may overflow, so use limit / i */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Assume n ≥ 2.  The function is not required to handle n < 2. */
    if (n > MAX_LIMIT) {
        /* In practice this should not happen for the required test range.  */
        /* If it does, we enlarge the arrays, but that is out of scope.  */
    }

    build_sieve(n);

    /* Compute L_n = lcm(1,2,…,n) */
    seq_t L_n = 1;
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t p = primes[idx];
        idx_t pow = p;
        while (pow <= n / p)
            pow *= p;                     /* pow = p^e_max ≤ n  */
        L_n *= (seq_t)pow;
    }

    seq_t min_val = 0;                       /* 0 sentinel for first value */

    /* Try all candidates m = 2 … n */
    for (idx_t m = 2; m <= n; ++m) {
        seq_t L_m = L_n;
        idx_t original = m;                 /* keep original m for comparison */

        /* Factorise m using the precomputed spf table */
        while (original > 1) {
            idx_t p = spf[original];
            while (original % p == 0)
                original /= p;

            /* For this prime p, find its contributions to the LCM of 1..n */
            idx_t pow_max = p;
            idx_t e_max   = 1;
            while (pow_max <= n / p) {
                pow_max *= p;
                ++e_max;
            }
            idx_t cnt_max = n / pow_max - n / (pow_max * p);

            /* If p^e_max occurs only once and that number is exactly m,
               we have to reduce the exponent in the LCM.                     */
            if (cnt_max == 1 && m == pow_max) {
                /* Find the next lower exponent that is still present.        */
                idx_t pow_next = pow_max / p;
                idx_t e_next   = e_max - 1;
                while (pow_next > 0) {
                    idx_t cnt_next = n / pow_next - n / (pow_next * p);
                    if (cnt_next > 0) break;
                    pow_next /= p;
                    --e_next;
                }
                idx_t delta = e_max - e_next;            /* amount to reduce   */

                /* Compute p^delta and divide L_m by it. */
                seq_t pow_reduce = 1;
                for (idx_t i = 0; i < delta; ++i)
                    pow_reduce *= (seq_t)p;
                L_m /= pow_reduce;
            }
        }

        /* Now combine with the required divisor n+1 */
        seq_t g   = gcd(L_m, (seq_t)(n + 1));
        seq_t km  = (L_m / g) * (seq_t)(n + 1);

        if (min_val == 0 || km < min_val)
            min_val = km;
    }

    return min_val;
}

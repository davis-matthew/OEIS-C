/*
 * A281528  a(n) = least numerator k such that the proper fraction k/n needs
 *          three or more terms as a signed sum of distinct unit fraction, or
 *          0 if no such numerator exists.
 * https://oeis.org/A281528
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least numerator k (1 ≤ k < n) such that the proper
 *            fraction k/n cannot be expressed as a sum of at most two
 *            distinct unit fractions (with signs). Returns 0 if every 1 ≤ k <
 *            n has such a 2‑term representation
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 74 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>   /* for bool */

static bool two_term_representation(idx_t n, idx_t k)
{
    /* one‑term case */
    if ((n % k) == 0) return true;

    /* --- sum of two positive unit fractions (1/d1 + 1/d2) --- */
    /* solve k*d1*d2 = n*(d1 + d2)  ->  d2 = n*d1 / (k*d1 - n)  (k*d1 > n) */
    unsigned long long n64 = (unsigned long long)n;
    unsigned long long k64 = (unsigned long long)k;
    unsigned long long d1_start = n64 / k64 + 1ULL;
    unsigned long long d1_max   = (2ULL * n64) / k64;   /* when d2 < d1 */
    if (d1_start <= d1_max) {
        for (unsigned long long d1 = d1_start; d1 <= d1_max; ++d1) {
            unsigned long long denom = k64 * d1 - n64;    /* >0 because d1 > n/k */
            if (denom == 0) continue;                   /* safety */
            unsigned long long numer = n64 * d1;
            if (numer % denom != 0) continue;
            unsigned long long d2 = numer / denom;
            if (d2 <= d1) break;                        /* further d1 will give even smaller d2 */
            if (d2 != d1) return true;                  /* distinct denominators found */
        }
    }

    /* --- difference of two unit fractions (1/d1 - 1/d2, d2 > d1) --- */
    /* solve k*d1*d2 = n*(d2 - d1)  ->  t = (k*d1^2)/(n - k*d1)  with t > 0 integer */
    unsigned long long max_d1 = (n64 - 1ULL) / k64;     /* since k*d1 < n */
    for (unsigned long long d1 = 1ULL; d1 <= max_d1; ++d1) {
        unsigned long long denom = n64 - k64 * d1;       /* >0 */
        if (denom == 0) continue;                       /* should not happen */
        unsigned long long prod  = k64 * d1 * d1;        /* k*d1^2 */
        if (prod % denom == 0) return true;             /* t is integer, gives valid d2 */
    }

    return false; /* no 2‑term representation found */
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* out of defined range – safe default */

    for (idx_t k = 1; k < n; ++k) {
        if (two_term_representation(n, k)) continue;
        return (seq_t)k;          /* first k without 2‑term representation */
    }
    return 0;                     /* all k have a 2‑term representation */
}

/*
 * A280274  a(n) is the maximum value in row n of A279907.
 * https://oeis.org/A280274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{p|n} ceil( floor(log_p(n)) / e_p(n) ). For n = 1,
 *            a(1) = 0. `e_p(n)` is the exponent of prime `p` in the prime
 *            factorisation of `n`
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9485 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include needed standard headers */
#include <stdint.h>

/* Return the integer floor of log base p of n (p >= 2). */
static idx_t floor_log_p(idx_t n, idx_t p)
{
    /* We compute the largest k such that p^k <= n.
       Use unsigned __int128 to avoid overflow. */
    unsigned __int128 power = 1;
    idx_t k = 0;
    while (power <= (unsigned __int128)n / (unsigned __int128)p) {
        power = power * (unsigned __int128)p;
        ++k;
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    idx_t original = n;          /* preserve original n for log computations */
    seq_t best = 0;              /* maximum rho found so far */

    /* Factor out 2 */
    if ((n & 1ULL) == 0) {
        idx_t e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        idx_t amax = floor_log_p(original, 2);
        /* ceil(amax / e) */
        idx_t rho = (amax + e - 1) / e;
        if ((seq_t)rho > best) best = (seq_t)rho;
    }

    /* Factor remaining odd primes */
    for (idx_t p = 3; p <= n; p += 2) {
        if (p * p > n) break;   /* remaining n will be prime if > 1 */

        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            idx_t amax = floor_log_p(original, p);
            idx_t rho = (amax + e - 1) / e;
            if ((seq_t)rho > best) best = (seq_t)rho;
        }
    }

    /* If n > 1 then it is a prime factor with exponent 1 */
    if (n > 1) {
        idx_t p = n;
        idx_t e = 1;
        idx_t amax = floor_log_p(original, p);
        idx_t rho = (amax + e - 1) / e;    /* e = 1 so rho = amax */
        if ((seq_t)rho > best) best = (seq_t)rho;
    }

    return best;
}

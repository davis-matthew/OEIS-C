/*
 * A339423  If n = p_1 * ... * p_m with primes p_i <= p_{i+1}, a(n) =
 *          Sum_{k<m} Product_{j <= k} p_j.
 * https://oeis.org/A339423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{m-1} Π_{j=1}^{k} p_j, where n = p₁⋯p_m (p_i
 *            non‑decreasing primes, multiplicity counted)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(1) = 0, a(0) not used */

    uint64_t x = (uint64_t)n;      /* work in 64‑bit for factorisation */
    uint64_t pr[64];               /* prime factors (ascending) */
    int      ex[64];               /* corresponding exponents */
    size_t   cnt = 0;

    /* ---------- factorise n ---------- */
    while (x > 1) {
        uint64_t p = _smallest_prime_factor(x);
        int e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        pr[cnt] = p;
        ex[cnt] = e;
        ++cnt;
    }

    /* ---------- compute total number of prime occurrences ----------- */
    idx_t total_m = 0;
    for (size_t i = 0; i < cnt; ++i)
        total_m += ex[i];

    if (total_m <= 1)          /* n is prime or n==1 */
        return 0;

    /* ---------- accumulate the required sum ------------------------ */
    seq_t prod   = 1;          /* running product of first k primes */
    seq_t sum    = 0;          /* result to be returned */
    idx_t done   = 0;          /* number of primes processed so far */

    for (size_t i = 0; i < cnt; ++i) {
        for (int j = 0; j < ex[i]; ++j) {
            prod *= (seq_t)pr[i];
            ++done;
            if (done < total_m)          /* skip the final full product */
                sum += prod;
        }
    }
    return sum;
}

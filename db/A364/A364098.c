/*
 * A364098  Expansion of Sum_{k>0} k * x^(2*k-1) / (1 - x^(5*k-4)).
 * https://oeis.org/A364098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364098
   Formula: a(n) = (1/5) * Σ_{d | 5*n-3, d≡1 (mod 5)} (d+4)
   Normal implementation by factorising 5*n-3 and summing the
   appropriate divisors.  This runs in O(√n) per call, which is
   easily fast enough for n up to about 10 000.
*/

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 */
    if (n < 1) return 0;

    /* 5*n-3 always fits in a 64‑bit unsigned integer for the
       ranges that matter here. */
    uint64_t m = 5ULL * (uint64_t)n - 3ULL;

    /* Factorise m.  We will store prime factors and their
       exponents in a small fixed array. */
    struct { uint64_t p; int e; } fac[64];
    int fac_cnt = 0;
    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac[fac_cnt].p = p;
            fac[fac_cnt].e = e;
            ++fac_cnt;
        }
    }
    if (tmp > 1) {
        fac[fac_cnt].p = tmp;
        fac[fac_cnt].e = 1;
        ++fac_cnt;
    }

    /* Generate all divisors of m.  The number of divisors is at
       most a few hundred for 64‑bit numbers. */
    uint64_t *divs = (uint64_t*)malloc(sizeof(uint64_t));
    if (!divs) return 0;
    size_t divs_len = 1;
    size_t divs_cap = 1;
    divs[0] = 1;

    for (int i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac[i].p;
        int e = fac[i].e;
        size_t old_len = divs_len;
        uint64_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < old_len; ++j) {
                if (divs_len == divs_cap) {
                    divs_cap *= 2;
                    uint64_t *tmp2 = (uint64_t*)realloc(divs, divs_cap * sizeof(uint64_t));
                    if (!tmp2) { free(divs); return 0; }
                    divs = tmp2;
                }
                divs[divs_len++] = divs[j] * p_pow;
            }
        }
    }

    /* Sum (d+4)/5 over the divisors that are ≡1 (mod 5). */
    seq_t sum = 0;
    for (size_t i = 0; i < divs_len; ++i) {
        uint64_t d = divs[i];
        if (d % 5ULL == 1ULL) {
            sum += ((seq_t)d + (seq_t)4) / (seq_t)5;
        }
    }

    free(divs);
    return sum;
}

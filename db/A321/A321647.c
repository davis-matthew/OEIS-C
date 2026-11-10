/*
 * A321647  Number of distinct row/column permutations of the Ferrers diagram
 *          of the integer partition with Heinz number n.
 * https://oeis.org/A321647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008480(n) * A008480(A122111(n)), computed as
 *            multinomial(multiplicities of parts) * multinomial(gaps between
 *            distinct parts), where the parts are the prime indices of n (the
 *            second factor counts the conjugate partition)
 * Range:     u8 n <= 113, u16 n <= 853, u32 n <= 5262, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 9848 (2026-11-08)
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"

/* index of prime p (p must be prime): number of primes <= p */
static uint64_t A321647_prime_index(uint64_t p)
{
    uint64_t count = 0;
    for (uint64_t q = 2; q <= p; ++q) {
        bool isp = true;
        for (uint64_t d = 2; d * d <= q; ++d) {
            if (q % d == 0) { isp = false; break; }
        }
        if (isp) ++count;
    }
    return count;
}

/* multinomial (sum k_i)! / prod k_i! via product of binomials */
static seq_t A321647_multinomial(const uint64_t *k, size_t cnt)
{
    seq_t res = (seq_t)1;
    idx_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (idx_t)k[i];
        res = res * binomial(sum, (idx_t)k[i]);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t vals[64];
    uint64_t mult[64];
    for (size_t i = 0; i < cnt; ++i) {
        vals[i] = A321647_prime_index(primes[i]);
        mult[i] = exps[i];
    }
    /* insertion sort by part value */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t v = vals[i], m = mult[i];
        size_t j = i;
        while (j > 0 && vals[j - 1] > v) {
            vals[j] = vals[j - 1];
            mult[j] = mult[j - 1];
            --j;
        }
        vals[j] = v;
        mult[j] = m;
    }

    uint64_t gaps[64];
    uint64_t prev = 0;
    for (size_t i = 0; i < cnt; ++i) {
        gaps[i] = vals[i] - prev;
        prev = vals[i];
    }

    seq_t r1 = A321647_multinomial(mult, cnt);
    seq_t r2 = A321647_multinomial(gaps, cnt);
    return r1 * r2;
}

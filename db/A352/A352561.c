/*
 * A352561  a(n) = number of modules with n elements over the ring of integers
 *          in the imaginary quadratic field of discriminant -3.
 * https://oeis.org/A352561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime factors p^e of n of * if p = 3:
 *            partition(e) * if p ≡ 2 (mod 3): 0 if e odd else partition(e/2)
 *            * if p ≡ 1 (mod 3): sum_{i=0}^e partition(i)*partition(e-i) *
 *            (partition(k) is the number of integer partitions of k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used for offset 1, but safe */

    /* --- factor n: store distinct primes and exponents --- */
    /* maximum number of distinct primes for n <= 1e6 is tiny;
       we use a small static buffer that is definitely large enough. */
    const size_t MAX_PRIMES = 32;
    idx_t primes[MAX_PRIMES];
    idx_t exps[MAX_PRIMES];
    size_t pcnt = 0;

    idx_t m = n;
    for (idx_t i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            primes[pcnt] = i;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (m > 1) {                     /* remaining prime factor */
        primes[pcnt] = m;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* --- determine maximum exponent needed for partition table --- */
    idx_t max_e = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        idx_t e = exps[i];
        idx_t need = 0;
        if (p == 3)
            need = e;
        else if (p % 3 == 2)
            need = e / 2;            /* integer division, odd exponents will fail later */
        else if (p % 3 == 1)
            need = e;
        if (need > max_e) max_e = need;
    }

    /* --- compute partition numbers up to max_e --- */
    seq_t *part = (seq_t *)malloc((max_e + 1) * sizeof(seq_t));
    if (!part) return 0;            /* allocation failed, return 0 as fallback */
    part[0] = (seq_t)1;
    for (idx_t nidx = 1; nidx <= max_e; ++nidx) {
        seq_t total = 0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)(k * (3 * k - 1) / 2);
            if (pent1 > nidx) break;
            seq_t val = part[nidx - pent1];
            if (k & 1) total += val; else total -= val;
            idx_t pent2 = (idx_t)(k * (3 * k + 1) / 2);
            if (pent2 > nidx) continue;
            val = part[nidx - pent2];
            if (k & 1) total += val; else total -= val;
        }
        part[nidx] = total;
    }

    /* --- compute the product of contributions --- */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        idx_t e = exps[i];
        seq_t contrib = 0;

        if (p == 3) {
            contrib = part[e];
        } else if (p % 3 == 2) {
            if (e & 1ULL) {          /* odd exponent => impossible */
                free(part);
                return (seq_t)0;
            }
            contrib = part[e / 2];
        } else if (p % 3 == 1) {
            seq_t sum = 0;
            for (idx_t a = 0; a <= e; ++a) {
                sum += part[a] * part[e - a];
            }
            contrib = sum;
        } else {
            contrib = (seq_t)1;       /* shouldn't occur */
        }

        result *= contrib;
    }

    free(part);
    return result;
}

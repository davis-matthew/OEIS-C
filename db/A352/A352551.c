/*
 * A352551  a(n) = number of modules with n elements over the ring of integers
 *          in the real quadratic field of discriminant 8.
 * https://oeis.org/A352551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_p f_p(v_p(n)) where f_p(e) = • p = 2 : f_2(e) = p(e)
 *            (partition number) • p ≡ 1 or 7 (mod 8) : f_p(e) = Σ_{k=0..e}
 *            p(k)·p(e−k) • otherwise (p ≡ 3 or 5 (mod 8)) : f_p(e) = p(e/2)
 *            if e even, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *partitions = NULL;   /* array of partition numbers */
static size_t part_max = 0;        /* maximum exponent precomputed */

/* Ensure the array of partition numbers is computed up to max_exp. */
static void ensure_partitions(size_t max_exp)
{
    if (max_exp <= part_max) return;
    if (partitions) free(partitions);
    partitions = (seq_t*)malloc((max_exp + 1) * sizeof(seq_t));
    if (!partitions) return;   /* allocation failed → a() will return 0 */

    partitions[0] = 1;
    for (size_t i = 1; i <= max_exp; ++i) partitions[i] = 0;
    for (size_t i = 1; i <= max_exp; ++i)
        for (size_t j = i; j <= max_exp; ++j)
            partitions[j] += partitions[j - i];
}

/* Compute result for a single exponent e and prime p. */
static seq_t contrib_for_prime(idx_t p, size_t e)
{
    if (p == 2) return partitions[e];
    if ((p % 8 == 1) || (p % 8 == 7)) {
        seq_t sum = 0;
        for (size_t k = 0; k <= e; ++k)
            sum += partitions[k] * partitions[e - k];
        return sum;
    }
    /* inert prime */
    if (e & 1) return 0;
    return partitions[e / 2];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* below offset – undefined */
    /* Primes sufficient for n ≤ 10 000. */
    static const idx_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };
    const size_t PRIME_CNT = sizeof(primes) / sizeof(primes[0]);

    /* Store factor pairs (prime, exponent). */
    idx_t pf[20];      /* enough for n ≤ 10 000 (max 10 distinct primes) */
    size_t pf_cnt = 0;
    idx_t m = n;
    size_t max_exp = 0;

    for (size_t i = 0; i < PRIME_CNT && primes[i] * primes[i] <= m; ++i) {
        idx_t p = primes[i];
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pf[pf_cnt * 2] = p;
            pf[pf_cnt * 2 + 1] = e;
            ++pf_cnt;
            if (e > max_exp) max_exp = e;
            if (p == 2 && e / 2 > max_exp) max_exp = e / 2;
        }
    }
    if (m > 1) {               /* remaining prime factor */
        pf[pf_cnt * 2] = m;
        pf[pf_cnt * 2 + 1] = 1;
        ++pf_cnt;
        if (1 > max_exp) max_exp = 1;
    }

    if (max_exp < 1) max_exp = 1;
    ensure_partitions(max_exp);
    if (!partitions) return 0; /* allocation failure */

    seq_t result = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t p = pf[i * 2];
        size_t e = (size_t)pf[i * 2 + 1];
        seq_t contrib = contrib_for_prime(p, e);
        result *= contrib;
        if (result == 0) break;
    }
    return result;
}

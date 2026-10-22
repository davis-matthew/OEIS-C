/*
 * A304327  Number of ways to write n as a product of a perfect power and a
 *          squarefree number.
 * https://oeis.org/A304327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct perfect‑power factors a^b (a>1, b≥2)
 *            such that n/(a^b) is square‑free. The perfect power 1^b is
 *            counted once when n itself is square‑free
 * Range:     u8 n <= 8996, u16 n <= 8996, u32 n <= 8996, u64 n <= 8996,
 *            u128 n <= 8996, big any n
 * Helpers:   cmp_seq_t, factor_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The sequence utilities are available from the framework. */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/is_squarefree.h" /* bool is_squarefree(seq_t x); */
#include "../utils/cmp_seq_t.h"   /* int cmp_seq_t(const void *p1, const void *p2); */

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n <= 1)
        return 1;  /* a(1) = 1 */

    /* Factor n.  We use at most 64 distinct primes (more than enough 
       for uint64_t). */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    /* Find maximum exponent and check square‑freeness. */
    size_t max_exp = 0;
    bool is_squarefree = true;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > max_exp)
            max_exp = ex[i];
        if (ex[i] > 1)
            is_squarefree = false;
    }

    /* If n is square‑free, the only valid representation is 1^b * n.
       Count it once. */
    if (is_squarefree)
        return 1;

    /* Collect all valid perfect‑power values a^b.
       There are at most (max_exp - 1) of them. */
    seq_t values[64];
    size_t vlen = 0;
    seq_t n_seq = (seq_t)n;

    for (size_t b = 2; b <= max_exp; ++b) {
        bool ok = true;
        seq_t s = 1; /* square‑free part */

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t e = ex[i];
            uint64_t r = e % b;
            if (r > 1) {   /* invalid exponent for this b */
                ok = false;
                break;
            }
            if (r == 1)
                s *= (seq_t)pr[i];
        }

        if (!ok)
            continue;

        /* a^b = n / s  (integer division, s divides n). */
        seq_t a_pow = n_seq / s;
        values[vlen++] = a_pow;
    }

    /* Sort and deduplicate the list of perfect‑power values. */
    if (vlen == 0)
        return 0; /* Should not happen for non‑square‑free n */

    qsort(values, vlen, sizeof(seq_t), cmp_seq_t);

    size_t unique = 0;
    seq_t last = 0;
    bool first = true;
    for (size_t i = 0; i < vlen; ++i) {
        if (first || cmp_seq_t(&values[i], &last) != 0) {
            ++unique;
            last = values[i];
            first = false;
        }
    }

    return unique;
}

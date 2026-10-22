/*
 * A352056  Sum of the 10th powers of the divisor complements of the odd
 *          proper divisors of n.
 * https://oeis.org/A352056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10 * Σ_{d|n, d<n, d odd} 1/d^10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7131,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* needed types and helpers */
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/factor_u64.h"  /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* 1. Strip all factors of 2 to obtain the odd part m of n. */
    idx_t m = n;
    while ((m & 1ULL) == 0ULL)
        m >>= 1;

    /* 2. Factorise m (it is odd and ≤ n). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* 3. Enumerate all divisors of m.  They are exactly the odd divisors of n. */
    uint64_t divs[256];
    size_t sz = 1;          /* sz = number of divisors collected so far */
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        size_t cur = sz;
        uint64_t p_pow = 1;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            p_pow *= p;
            for (size_t j = 0; j < cur; ++j)
                divs[sz++] = divs[j] * p_pow;
        }
    }

    /* 4. Sum (n/d)^10 over all odd divisors d of n that are proper (d < n). */
    seq_t sum = 0;
    for (size_t k = 0; k < sz; ++k) {
        uint64_t d = divs[k];
        if (d == n)            /* skip the divisor n itself (proper divisors only) */
            continue;
        idx_t t = n / d;       /* t = n/d.  Stored in idx_t to avoid overflow. */
        seq_t t10 = big_pow((seq_t)t, (seq_t)10); /* compute t^10 in seq_t */
        sum += t10;            /* accumulate */
    }
    return sum;
}

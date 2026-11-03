/*
 * A363878  Number of divisors of 7*n-4 of form 7*k+2.
 * https://oeis.org/A363878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of (7*n-4) with d ≡ 2 (mod 7)
 * Range:     u8 n <= 39, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive helper to count divisors with a given residue modulo 7 */
static void
count_mod7_divs(const uint64_t *pr, const uint64_t *exp, size_t dim, size_t pos,
                int residue, uint64_t *cnt)
{
    if (pos == dim) {
        if (residue == 2)
            (*cnt)++;
        return;
    }

    int pmod = (int)(pr[pos] % 7ULL);
    int rpow = 1;            /* pmod^e mod 7, starts with e = 0 */
    for (uint64_t e = 0; e <= exp[pos]; ++e) {
        /* Use current residue */
        count_mod7_divs(pr, exp, dim, pos + 1, (residue * rpow) % 7, cnt);
        /* Prepare for next exponent */
        rpow = (rpow * pmod) % 7;
    }
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but return 0 */
        return (seq_t)0;

    /* m = 7*n - 4 */
    seq_t m = ((seq_t)7 * (seq_t)n) - (seq_t)4;

    /* prime factorisation of m (u64 fast routine) */
    /* Max number of distinct prime factors for m <= 7*10000-4 < 70000
       is small (<10), so allocate a buffer of 32. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* Count divisors with residue 2 (mod 7) */
    uint64_t result = 0;
    count_mod7_divs(primes, exps, cnt, 0, 1 % 7, &result);

    return (seq_t)result;
}

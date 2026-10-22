/*
 * A309171  a(n) = Product_{d|n} Stirling2(n,d).
 * https://oeis.org/A309171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} Stirling2(n, d)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/_smallest_prime_factor.h"

/* Compute a(n) = \prod_{d | n} S(n, d) where S(n,d) is the Stirling
   number of the second kind.  The computation runs in O(n^2) time and
   uses only O(n) additional memory. */
seq_t a(idx_t n)
{
    /* n == 0 is not defined in OEIS; return 0 for safety */
    if (n == 0) {
        seq_t zero = 0;
        return zero;
    }

    uint64_t nn = (uint64_t)n;          /* for pointer arithmetic */
    /* ---------- factor n and build its set of divisors ---------- */
    size_t div_count = 1;
    uint64_t *divs = (uint64_t *)malloc(sizeof(uint64_t));
    divs[0] = 1;

    uint64_t tmp = nn;
    while (tmp > 1) {
        uint64_t p = _smallest_prime_factor(tmp);
        size_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }

        size_t old = div_count;
        size_t new_count = old * (exp + 1);
        uint64_t *new_divs = (uint64_t *)malloc(new_count * sizeof(uint64_t));

        size_t idx = 0;
        for (size_t i = 0; i < old; ++i) {
            uint64_t base = divs[i];
            uint64_t mul  = 1;
            for (size_t e = 0; e <= exp; ++e) {
                new_divs[idx++] = base * mul;
                mul *= p;
            }
        }
        free(divs);
        divs      = new_divs;
        div_count = new_count;
    }

    /* ---------- compute Stirling numbers row n using one array ---------- */
    seq_t *s = (seq_t *)malloc((nn + 2) * sizeof(seq_t));
    for (size_t i = 0; i <= nn + 1; ++i) s[i] = 0;
    s[0] = 1;                 /* S(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t k = i; k >= 1; --k) {
            s[k] = ((seq_t)k * s[k]) + s[k - 1];
        }
        s[0] = 0;              /* S(i,0) = 0 for i > 0 */
    }

    /* ---------- multiply required Stirling numbers ---------- */
    seq_t product = 1;
    for (size_t i = 0; i < div_count; ++i) {
        product = product * s[divs[i]];
    }

    /* ---------- clean up ---------- */
    free(divs);
    free(s);

    return product;
}

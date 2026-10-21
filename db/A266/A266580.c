/*
 * A266580  Numerator of the continued fraction [n; p(1), p(2), p(3),...,
 *          p(m)], where n = Product_{k=1..m} p(k), m = bigomega(n),
 *          p(1)<=p(2)<=p(3)<=...<=p(m-1)<=p(m), sigma_0(p(k)) = 2.
 * https://oeis.org/A266580
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n * num + den ; where num/den is the continued fraction
 *            [p(1); p(2), ..., p(m)] of the prime factors of n with
 *            multiplicity and sorted increasingly
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* sequence defined for n ≥ 2 */

    /* Prime factorisation of n.  we store each prime factor with multiplicity
       in the array 'pf', which is large enough for any n that fits in
       idx_t (64‑bit).  */
    uint64_t pf[128];
    size_t pf_cnt = 0;
    uint64_t remaining = n;

    /* handle factor 2 */
    while ((remaining & 1ULL) == 0) {
        pf[pf_cnt++] = 2;
        remaining >>= 1;
    }

    /* test odd divisors up to sqrt(remaining) */
    for (uint64_t d = 3; d * d <= remaining; d += 2) {
        while (remaining % d == 0) {
            pf[pf_cnt++] = d;
            remaining /= d;
        }
    }

    /* if anything remains it is a prime > sqrt(original n) */
    if (remaining > 1) {
        pf[pf_cnt++] = remaining;
    }

    /* continued fraction evaluation: start from the last prime */
    seq_t num = (seq_t)pf[pf_cnt - 1];
    seq_t den = 1;

    for (long long i = (long long)pf_cnt - 2; i >= 0; --i) {
        seq_t prime = (seq_t)pf[i];
        seq_t new_num = prime * num + den;
        seq_t new_den = num;
        num = new_num;
        den = new_den;
    }

    /* final result: a(n) = n * num + den */
    return (seq_t)n * num + den;
}

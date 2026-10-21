/*
 * A251758  Let n>=2 be a positive integer with divisors 1 = d_1 < d_2 < ... <
 *          d_k = n, and s = d_1*d_2 + d_2*d_3 + ... + d_(k-1)*d_k. The
 *          sequence lists the values a(n) = floor(n^2/s).
 * https://oeis.org/A251758
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n² / s), s = Σ d_i · d_{i+1} over the sorted
 *            divisors of n
 * Range:     u8 n <= 15, u16 n <= 824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, factor_u64, is_prime, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 2 no value is defined, but the function ignores those calls. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            /* index below the OEIS offset */

    /* Primes are trivial: s = 1·n and a(n) = n. */
    if (is_prime(n))
        return (seq_t)n;

    /* Factor n.  The buffer sizes are ample for n ≤ 10⁴. */
    uint64_t primes[64], exps[64];
    size_t pf_cnt = 0;
    factor_u64(n, primes, exps, &pf_cnt);

    /* Count total number of divisors. */
    size_t div_cnt = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        div_cnt *= (exps[i] + 1);

    /* Allocate array for all divisors (seq_t may be arbitrary‑precision). */
    seq_t *divs = seq_alloc_new_array(div_cnt);
    if (!divs)
        return (seq_t)0;            /* allocation failure */

    /* Generate all divisors. */
    divs[0] = (seq_t)1;
    size_t cur = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p  = primes[i];
        seq_t  ppow = (seq_t)1;
        size_t before = cur;      /* number of divisors before adding this prime */
        for (size_t e = 0; e < exps[i]; ++e) {
            ppow *= p;                  /* p⁰, p¹, …, pᵉ */
            for (size_t j = 0; j < before; ++j)
                divs[cur++] = divs[j] * ppow;
        }
    }

    /* Sort the divisor list. */
    qsort(divs, div_cnt, sizeof(seq_t), cmp_seq_t);

    /* Compute s = Σ d_i·d_{i+1}. */
    seq_t s = (seq_t)0;
    for (size_t i = 0; i + 1 < div_cnt; ++i)
        s += divs[i] * divs[i + 1];

    seq_t nn   = (seq_t)n * (seq_t)n;
    seq_t res  = nn / s;              /* floor division is implicit */

    seq_free_array(divs);
    return res;
}

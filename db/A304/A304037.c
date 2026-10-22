/*
 * A304037  If n = Product (p_j^k_j) then a(n) = Sum (pi(p_j)^k_j), where pi()
 *          = A000720.
 * https://oeis.org/A304037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^k‖n} ( pi(p) )^k, where pi(p) = #{ primes ≤ p }.
 *            The OEIS definition gives a(1)=0
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 0 */
        return 0;

    /* Basic trial‑division factorization with a fixed buffer.
       factor_u64( n, primes, exps, &cnt ) fills the arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* pi(p) counts primes ≤ p. The helper compute_pi
           returns a seq_t value. */
        seq_t pi_val = compute_pi(primes[i]);

        /* Compute pi_val^exps[i] */
        seq_t power = 1;
        for (uint64_t e = 0; e < exps[i]; ++e)
            power *= pi_val;

        sum += power;
    }
    return sum;
}

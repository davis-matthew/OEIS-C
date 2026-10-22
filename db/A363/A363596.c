/*
 * A363596  a(n) = (Product_{k=1..pi(n+1)} prime(k)^floor(n/(prime(k)-1) )
 *          )/(n+1)!.
 * https://oeis.org/A363596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Product_{p≤n+1} p^⌊n/(p-1)⌋)/(n+1)!
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 111, u64 n <= 359,
 *            u128 n <= 1259, big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Utilities needed for the computation */
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/*
 * Compute OEIS A363596:
 *   a(n) = Π_{p prime ≤ n+1} p^⌊n/(p-1)⌋  / (n+1)!
 */
seq_t a(idx_t n)
{
    /* Get primes up to n+1 (hidden limit includes n+1 itself) */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n + 1, &primes, &prime_cnt);

    seq_t result = 1;
    idx_t fact_n1 = n + 1;       /* (n+1)! part */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];

        /* Exponent from product part: floor(n/(p-1)) */
        idx_t e_prod = n / (p - 1);

        /* Exponent of p in (n+1)! using Legendre's formula */
        idx_t e_fact = 0;
        idx_t power = p;
        while (power <= fact_n1) {
            e_fact += fact_n1 / power;
            if (power > fact_n1 / p) break;  /* avoid overflow */
            power *= p;
        }

        /* Net exponent in a(n) */
        idx_t e = (e_prod > e_fact) ? (e_prod - e_fact) : 0;
        if (e > 0) {
            seq_t part = seq_pow((seq_t)p, e);
            result = seq_mul(result, part);
        }
    }

    free(primes);
    return result;
}

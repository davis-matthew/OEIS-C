/*
 * A321812  Sum of 8th powers of odd divisors of n.
 * https://oeis.org/A321812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_8(odd part of n) = product over odd prime powers
 *            p^e in the odd part of n of (1 + p^8 + p^16 + … + p^(8e))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* remove all factors of 2 (the ‘odd part’ of n) */
    uint64_t odd = (uint64_t)n;
    while ((odd & 1ULL) == 0) odd >>= 1;

    seq_t res = (seq_t)1;          /* product of sums over prime powers */

    if (odd == 1)                  /* n is a power of 2 */
        return res;

    /* factorize the odd part */
    size_t cnt;
    uint64_t p[64], e[64];        /* 64 slots are more than enough for 64‑bit numbers */
    factor_u64(odd, p, e, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime = p[i];
        seq_t prime_seq = (seq_t)prime;

        /* compute p^8 once */
        seq_t p8 = big_pow(prime_seq, (seq_t)8);

        /* sum_{k=0}^{e} p^{8k}  (geometric series) */
        seq_t sum = (seq_t)1;            /* the 1 corresponding to k = 0 */
        seq_t cur = p8;                  /* current power p^{8k} for the loop */
        uint64_t exp = e[i];
        for (uint64_t j = 0; j < exp; ++j) {
            sum += cur;                  /* add p^{8k} */
            cur *= p8;                   /* advance to p^{8(k+1)} */
        }

        res *= sum;                      /* multiply contributions of distinct primes */
    }

    return res;
}

/*
 * A359262  a(n) is the largest number m such that prime(n)^m is in A359260.
 * https://oeis.org/A359262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest m such that for all k = 1..m, the arithmetic mean
 *            of * {1, p, p², …, pᵏ} is integer, where p = prime(n).
 *            Equivalently * for all such k: p^(k+1) ≡ 1 (mod (p-1)(k+1))
 * Range:     u8 n <= 17, u16 n <= 2498, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;        /* a(1) = 0 by definition */

    seq_t p = (seq_t)get_prime(n);      /* prime(n) as seq_t */
    seq_t m = 0;                        /* current value of a(n) */

    for (idx_t k = 1; ; ++k) {
        seq_t mod = (p - 1) * (k + 1);
        seq_t val = mod_pow(p, k + 1, mod);  /* p^(k+1) mod mod */

        if (val == 1)      /* p^(k+1) ≡ 1 (mod mod) holds */
            ++m;
        else                /* condition fails, stop */
            break;
    }
    return m;
}

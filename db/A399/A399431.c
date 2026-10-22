/*
 * A399431  a(n) = (2^prime(n) - 2)/prime(n) + 2.
 * https://oeis.org/A399431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^prime(n) - 2)/prime(n) + 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, get_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For seq_t that cannot hold 2^prime(n) (e.g. 64‑bit) we use a
   binomial expansion that keeps all intermediates within 128 bits. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset handling – not used for OEIS term */

    /* obtain the n-th prime (prime(1)=2) */
    uint32_t p = (uint32_t)get_prime(n);

    /* 64‑bit or smaller seq_t: compute via binomial coefficients */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 sum  = 0;     /* running sum of C(p,k)/p      */
        unsigned __int128 binom = 1;     /* C(p,0)                        */

        for (uint32_t k = 1; k < p; ++k) {
            binom = binom * (p - k + 1) / k;   /* next binomial coefficient */
            sum += binom / p;                 /* add integer quotient */
        }
        return (seq_t)sum + 2;                /* final result                */
    }

    /* arbitrary‑precision seq_t: use big_pow for 2^p directly */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)p);
    seq_t val  = (pow2 - (seq_t)2) / (seq_t)p; /* (2^p - 2)/p              */
    return val + (seq_t)2;
}

/*
 * A323060  a(n) = R_(prime(n)#) / Product_{j=1..n} R_(prime(j)), where
 *          prime(n)# is the n-th primorial number A002110(n) and R_k = (10^k
 *          - 1)/9.
 * https://oeis.org/A323060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = R_(prime(n)#) / Product_{j=1..n} R_(prime(j)) where R_k =
 *            (10^k - 1)/9 (in OEIS notation)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

/* Return repunit of length len (len fits in uint64_t) */
static seq_t repunit(seq_t len) {
    seq_t tenk = pow10((idx_t)len);   /* 10^len */
    seq_t num  = tenk - (seq_t)1;     /* 10^len – 1 */
    return num / (seq_t)9;            /* (10^len – 1)/9 */
}

/* Very small prime generator: returns the nth prime (1‑based). */
static uint64_t nth_prime(idx_t n) {
    if (n == 0) return 0;          /* not used for offset 1 */
    idx_t count = 0;
    uint64_t cand = 2;
    while (count < n) {
        bool prime = true;
        for (uint64_t d = 2; d * d <= cand; ++d) {
            if (cand % d == 0) { prime = false; break; }
        }
        if (prime) {
            ++count;
            if (count == n) return cand;
        }
        ++cand;
    }
    return 0; /* unreachable */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined below offset 1 */
    seq_t result   = 1;
    seq_t primorial = 1;            /* product of primes seen so far */
    seq_t rep_prim = 1;            /* R_primorial ; R_1 = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        uint64_t p       = nth_prime(i);
        seq_t   p_seq    = (seq_t)p;
        seq_t   new_prim = primorial * p_seq;

        seq_t rep_prime   = repunit((seq_t)p);
        seq_t denom       = rep_prim * rep_prime;

        seq_t rep_new     = repunit(new_prim);
        seq_t factor      = rep_new / denom;

        result = result * factor;

        /* prepare for next iteration */
        primorial = new_prim;
        rep_prim  = rep_new;
    }
    return result;
}

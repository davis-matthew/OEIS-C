/*
 * A274851  Smallest prime q larger than p=prime(n) such that (p+1)(q+1) is a
 *          square m^2; a(n)=0 if there is no such q.
 * https://oeis.org/A274851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274851:
   a(n) = the smallest prime q > prime(n) such that (prime(n)+1)(q+1) is a perfect square.
   If no such q exists, a(n)=0.  The computation is performed by explicit
   factorisation of prime(n)+1 and a simple search for the first suitable q. */

/* Dependencies from the shared utils */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k) */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n) */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x) */

seq_t a(idx_t n)
{
    /* 1. Retrieve the nth prime p */
    seq_t p_seq = (seq_t)get_prime(n);
    if (p_seq < 2) return 0;      /* safety guard */

    /* 2. Compute p+1 as a 64‑bit integer for factorisation */
    uint64_t p  = (uint64_t)p_seq;
    uint64_t P  = p + 1;           /* p+1 */

    /* 3. Factor P to obtain its square‑free part d */
    uint64_t temp = P;
    uint64_t d    = 1;             /* square‑free part */
    for (idx_t k = 1; /*no init*/ ; ++k) {
        uint32_t prime = get_prime(k);
        uint64_t pr   = (uint64_t)prime;
        if (pr * pr > temp) break; /* remaining part is prime if >1 */
        int cnt = 0;
        while (temp % pr == 0) {
            temp /= pr;
            ++cnt;
        }
        if (cnt & 1)                /* odd exponent → include in d */
            d *= pr;
    }
    if (temp > 1)                   /* leftover prime factor */
        d *= temp;

    /* 4. If d == 1, no suitable q exists (q+1 would have to be a square) */
    if (d == 1) return 0;

    /* 5. Compute a = sqrt((p+1)/d).  Because (p+1)/d is a perfect square. */
    seq_t a_seq = floor_sqrt((seq_t)(P / d));
    seq_t t = a_seq + 1;            /* start search with the next integer */

    seq_t d_seq = (seq_t)d;         /* promote to seq_t for later arithmetic */

    for (;;) {
        /* q = d * t^2 - 1 */
        seq_t tt   = t * t;
        seq_t prod = d_seq * tt;
        seq_t q    = prod - 1;

        if (q <= p_seq) {          /* should not happen once t>a, but guard */
            ++t;
            continue;
        }

        if (is_prime_big(q))
            return q;              /* found the required prime */

        ++t;                       /* try the next integer */
    }
}

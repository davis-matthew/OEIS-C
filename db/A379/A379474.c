/*
 * A379474  Number of prime factors of the form p^e || n : [p == 1 (mod 8), e
 *          == 1 (mod 4)] or [p == 5 (mod 8), e == -1 (mod 4)].
 * https://oeis.org/A379474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors p^e || n such that (p ≡ 1 (mod 8)
 *            and e ≡ 1 (mod 4)) or (p ≡ 5 (mod 8) and e ≡ 3 (mod 4))
 * Range:     u8 n <= 6258, u16 n <= 6258, u32 n <= 6258, u64 n <= 6258,
 *            u128 n <= 6258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External function declaration for the sequence. */
seq_t a(idx_t n)
{
    /* Input n is guaranteed to be ≥ 1 (OEIS offset). */
    if (n <= 1) {
        return 0;
    }

    /* Work only on a 64‑bit copy of n for factorisation. */
    uint64_t x   = (uint64_t)n;          /* the integer to factorise */
    seq_t   cnt  = 0;                    /* result to be returned */

    /* Count factor 2 separately – it never satisfies the condition since
       2 ≡ 2 (mod 8).  Skip it to keep the loop simpler. */
    while (x % 2 == 0) {
        x /= 2;
    }

    /* Trial division by odd numbers up to sqrt(x).  The loop uses
       division by the current candidate to avoid overflow of i*i */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if ((p & 7) == 1) {         /* p ≡ 1 (mod 8) */
                if ((exp & 3) == 1)      /* exp ≡ 1 (mod 4) */
                    ++cnt;
            } else if ((p & 7) == 5) { /* p ≡ 5 (mod 8) */
                if ((exp & 3) == 3)      /* exp ≡ 3 (mod 4) */
                    ++cnt;
            }
        }
    }

    /* If a prime factor remains larger than 1, its exponent is 1. */
    if (x > 1) {
        uint64_t p   = x;
        uint64_t exp = 1;
        if ((p & 7) == 1) {             /* p ≡ 1 (mod 8) */
            if ((exp & 3) == 1)          /* exp ≡ 1 (mod 4) */
                ++cnt;
        } else if ((p & 7) == 5) {      /* p ≡ 5 (mod 8) */
            if ((exp & 3) == 3)          /* exp ≡ 3 (mod 4) */
                ++cnt;
        }
    }

    return cnt;
}

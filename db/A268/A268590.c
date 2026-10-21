/*
 * A268590  a(n) = (3*C(4p,p) - 20*C(3p,p) + 54*C(2p,p) - 60) / p^7, where p =
 *          prime(n).
 * https://oeis.org/A268590
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*C(4p,p) - 20*C(3p,p) + 54*C(2p,p) - 60) / p^7, * where
 *            p = prime(n)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   binomial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/binomial.h"

/* 128‑bit binomial for small arguments (used only when seq_t is 64‑bit). */
static unsigned __int128 binom128(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 num = n - k + i;
        unsigned __int128 den = i;
        res = res * num / den;          /* division is exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 5) return 0;                  /* values below the offset are irrelevant */

    uint32_t p32 = get_prime(n);
    idx_t    p   = (idx_t)p32;            /* prime as idx_t */

    /* 64‑bit build: use 128‑bit arithmetic to avoid intermediate overflow. */
    if (sizeof(seq_t) == 8) {
        unsigned __int128 c44 = binom128(4 * p, p);
        unsigned __int128 c33 = binom128(3 * p, p);
        unsigned __int128 c22 = binom128(2 * p, p);

        unsigned __int128 num = (unsigned __int128)3 * c44
                              - (unsigned __int128)20 * c33
                              + (unsigned __int128)54 * c22
                              - (unsigned __int128)60;

        unsigned __int128 denom = p;
        for (int i = 1; i < 7; ++i) denom *= p;   /* denom = p^7 */

        unsigned __int128 result = num / denom;
        return (seq_t)result;                      /* cast to 64‑bit seq_t */
    }

    /* Arbitrary‑precision build: rely on the supplied binomial routine. */
    seq_t c44 = binomial(4 * p, p);
    seq_t c33 = binomial(3 * p, p);
    seq_t c22 = binomial(2 * p, p);

    seq_t num = 3 * c44;
    num -= 20 * c33;
    num += 54 * c22;
    num -= 60;

    seq_t denom = p;
    for (int i = 0; i < 6; ++i) denom *= p;     /* denom = p^7 */

    return num / denom;
}

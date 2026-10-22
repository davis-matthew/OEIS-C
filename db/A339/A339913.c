/*
 * A339913  a(n) = x/gcd(n,x), where x = 1+A060681(n).
 * https://oeis.org/A339913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1+A060681(n))/A323071(n) i.e. a(n) = x/gcd(n,x) * where
 *            x = 1 + (n - n/p) and p = smallest prime factor of n
 * Range:     u8 n <= 287, u16 n <= 8968, u32 n <= 8968, u64 n <= 8968,
 *            u128 n <= 8968, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdint.h>

/* Simple trial‑division smallest prime factor, reliable for n ≤ 10⁶⁰ʰ. */
static uint64_t spf_uint64(uint64_t n)
{
    if (n <= 1) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return i;
    }
    return n;   /* n is prime */
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;                 /* A060681(1) = 0, so x = 1 */
    uint64_t p = spf_uint64((uint64_t)n);
    seq_t n_seq = (seq_t)n;
    seq_t p_seq = (seq_t)p;
    seq_t np = n_seq / p_seq;             /* n/p */
    seq_t x  = n_seq - np + 1;            /* 1 + A060681(n) */
    seq_t g  = gcd(x, n_seq);             /* gcd(n,x) */
    return x / g;
}

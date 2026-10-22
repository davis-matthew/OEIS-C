/*
 * A334654  Number of equilateral triangles of edge length square root of 2n
 *          and having vertices in Z^4, one of which is the origin.
 * https://oeis.org/A334654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 96 * g(n), where g is multiplicative on odd integers.
 *            g(3^e) = (27·3^e − 54·e + 1) / 4 g(p^e) for odd prime p≠3, e = 1
 *            : g = 1 + (2p ± 1) (− if p≡1 (mod 3), + otherwise) e ≥ 2 : g = 1
 *            + S·H + P·(p+1) with S = (p^e − 1)/(p−1) H = 2p − 1 (p≡1 (mod
 *            3)) or 2p + 1 (p≡2 (mod 3)) P = p^{e−1} and...
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t power_u64(idx_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    for (idx_t i = 0; i < exp; ++i)
        result *= b;
    return result;
}

static seq_t prime_power_value(idx_t p, idx_t e)
{
    if (p == 3U) {
        seq_t pow3 = power_u64(3U, e);            /* 3^e   */
        seq_t num  = (seq_t)27 * pow3          /* 27·3^e */
                  - (seq_t)54 * (seq_t)e + 1;   /* −54e + 1 */
        return num / 4;                         /* integer division */
    }

    if (e == 1U) {
        seq_t H = (p % 3U == 1U) ? (seq_t)(2 * p - 1)
                                  : (seq_t)(2 * p + 1);
        return 1 + H;
    }

    /* e >= 2 and p != 3 */
    seq_t powP = power_u64(p, e);              /* p^e   */
    seq_t sumDiv = (powP - 1) / (seq_t)(p - 1);/* (p^e−1)/(p−1) */
    seq_t H = (p % 3U == 1U) ? (seq_t)(2 * p - 1)
                              : (seq_t)(2 * p + 1);
    seq_t powP_minus1 = powP / (seq_t)p;        /* p^{e-1}   */
    seq_t extra = powP_minus1 * (seq_t)(p + 1); /* p^{e-1}·(p+1) */

    return 1 + sumDiv * H + extra;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return 0;

    /* Reduce n to its odd part (a(2n)=a(n)) */
    idx_t odd = n;
    while ((odd & 1ULL) == 0ULL)
        odd >>= 1U;

    seq_t result = 1;
    idx_t m = odd;

    for (idx_t p = 3U; p * p <= m; p += 2U) {
        if (m % p == 0U) {
            idx_t e = 0U;
            while (m % p == 0U) {
                m /= p;
                ++e;
            }
            seq_t gp = prime_power_value(p, e);
            result *= gp;
        }
    }

    if (m > 1U) {
        seq_t gp = prime_power_value(m, 1U);
        result *= gp;
    }

    return (seq_t)96 * result;
}

/*
 * A363843  a(n) is the number of isomorphism classes of genus 3 hyperelliptic
 *          curves over the finite field of order prime(n).
 * https://oeis.org/A363843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined as * a(1) = 76, * for n > 1, * let p = prime(n)
 *            (the n‑th prime), * a(n) = 2p^5 + 2p^3 - 2 * - 2(p^2 - p)[p ≡ 3
 *            (mod 4)] * + 2(p - 1)[p > 3] * + 4[p ≡ 1 (mod 8)] * + 12[p ≡ 1
 *            (mod 7)] * + 2[p = 7] * + 2[p ≡ 1 or 5 (mod 12)]
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Compute a(n) for sequence A363843 */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)76;

    /* Fetch the n-th prime (uint32_t is sufficient for n up to 10⁴) */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Compute powers of p */
    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p5 = p3 * p2;   /* p^5 = p^3 * p^2 */

    /* Base expression */
    seq_t a_val = 2 * p5 + 2 * p3 - 2;

    /* Indicator flags */
    unsigned cond_p4_3  = (p32 % 4UL == 3UL);       /* p ≡ 3 (mod 4) */
    unsigned cond_gt3   = (p32 > 3UL);              /* p > 3 */
    unsigned cond_p8_1  = (p32 % 8UL == 1UL);       /* p ≡ 1 (mod 8) */
    unsigned cond_p7_1  = (p32 % 7UL == 1UL);       /* p ≡ 1 (mod 7) */
    unsigned cond_eq7   = (p32 == 7UL);             /* p = 7 */
    unsigned cond_p12_1or5 = (p32 % 12UL == 1UL || p32 % 12UL == 5UL);

    /* Apply the conditional terms */
    a_val -= (seq_t)2 * (p2 - p) * cond_p4_3;
    a_val += (seq_t)2 * (p - 1) * cond_gt3;
    a_val += (seq_t)4 * cond_p8_1;
    a_val += (seq_t)12 * cond_p7_1;
    a_val += (seq_t)2 * cond_eq7;
    a_val += (seq_t)2 * cond_p12_1or5;

    return a_val;
}

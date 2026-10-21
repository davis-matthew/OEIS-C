/*
 * A263320  Number of regular elements in Z_n[i].
 * https://oeis.org/A263320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative. a(2^k) = 1 + 2^(2k‑1) (k ≥ 1) for odd
 *            prime powers p^e if p ≡ 3 (mod 4) : a(p^e) = p^(2e) – p^(2(e‑1))
 *            + 1 if p ≡ 1 (mod 4) : a(p^e) = (p^(e‑1)(p‑1) + 1)^2 (for e = 1
 *            both formulas reduce to p²)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* integer power: base^exp  (exp is at most 128 – safe for 64‑bit n) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* outside offset – harmless for safety */
        return (seq_t)0;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    /* factor out powers of 2 */
    idx_t e2 = 0;
    while ((tmp & (idx_t)1) == 0) {   /* while tmp % 2 == 0 */
        tmp >>= 1;                    /* tmp /= 2; */
        ++e2;
    }
    if (e2 > 0) {
        seq_t term = (seq_t)1 + pow_seq((seq_t)2, (e2 << 1) - 1);
        result *= term;
    }

    /* factor odd primes up to sqrt(tmp) */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t term;
            if (p % 4 == 3) {                       /* p ≡ 3 mod 4 */
                seq_t p2e  = pow_seq((seq_t)p, exp << 1);          /* p^(2e)   */
                seq_t p2e1 = pow_seq((seq_t)p, ((exp - 1) << 1));  /* p^(2(e‑1)) */
                term = p2e - p2e1 + (seq_t)1;
            } else {                                 /* p ≡ 1 mod 4 */
                seq_t inv   = pow_seq((seq_t)p, exp - 1);         /* p^(e‑1)  */
                seq_t inner = inv * ((seq_t)p - (seq_t)1) + (seq_t)1;
                term = inner * inner;
            }
            result *= term;
        }
    }

    /* remaining prime > √n (if any) */
    if (tmp > 1) {
        /* for exp = 1 both formulas give p² */
        result *= pow_seq((seq_t)tmp, (idx_t)2);
    }

    return result;
}

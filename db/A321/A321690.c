/*
 * A321690  Approximations up to 2^n for the 2-adic integer log(5).
 * https://oeis.org/A321690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^{k+1} * 4^k / k (mod 2^n) * The series
 *            truncates modulo 2^n; terms with 2‑adic valuation >= n are zero
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static inline int tz_uint64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(x);
#else
    int cnt = 0;
    while ((x & 1) == 0) { x >>= 1; ++cnt; }
    return cnt;
#endif
}

/*  Modular inverse of an odd number modulo 2^nbits.  Uses Newton iteration.  */
static seq_t mod_inverse_2power(seq_t a, idx_t nbits)
{
    if (nbits == 0)               /* modulus 1, inverse only for a≡1 */
        return (seq_t)1;

    seq_t mod  = pow2(nbits);          /* 2^nbits   */
    seq_t mask = mod - (seq_t)1;       /* 2^nbits-1 */

    seq_t x = (seq_t)1;                /* initial approximation (odd ⇒ 1 is OK) */
    idx_t bits = 1;

    while (bits < nbits) {
        seq_t t  = (a * x) & mask;          /* a·x mod 2^nbits */
        seq_t t2 = ((seq_t)2 - t) & mask;    /* 2 – t  mod 2^nbits */
        x = (x * t2) & mask;                 /* refine approximation */
        bits <<= 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t mod  = pow2(n);           /* 2^n (the modulus) */
    seq_t mask = mod - (seq_t)1;    /* 2^n – 1  (bit mask) */
    seq_t sum  = 0;

    for (idx_t k = 1; k <= n; ++k) {
        int  v   = tz_uint64(k);                    /* v₂(k)                */
        idx_t exp = 2 * k - v;                     /* valuation exponent   */
        if (exp >= n) continue;                   /* zero modulo 2^n       */

        idx_t odd = k >> v;                       /* odd part of k        */
        seq_t term_pow = pow2(exp) & mask;        /* 2^{exp}  modulo 2^n  */
        seq_t invOdd   = mod_inverse_2power((seq_t)odd, n);
        seq_t term     = (term_pow * invOdd) & mask;

        if (k & 1)           /* k odd → add   */
            sum += term;
        else                 /* k even → subtract */
            sum -= term;

        sum &= mask;          /* keep only lower n bits */
    }
    return sum & mask;
}

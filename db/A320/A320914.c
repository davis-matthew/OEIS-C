/*
 * A320914  One of the three successive approximations up to 13^n for 13-adic
 *          integer 5^(1/3). This is the 7 (mod 13) case (except for n = 0).
 * https://oeis.org/A320914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13‑adic lift of the cube root of 5 with residue 7
 *            (Hensel's lemma). For n ≥ 1, a(n) is the unique k ∈ [1, 13ⁿ]
 *            with k ≡ 7 (mod 13) and k³ ≡ 5 (mod 13ⁿ)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == 0) return 0;
    /* a(1) = 7 – the unique cube‑root of 5 modulo 13. */
    if (n == 1) return 7;

    seq_t cur = 7;          /* current root modulo 13ⁿ  (n ≥ 1) */
    seq_t pow = 13;         /* 13ⁿ  ;  pow = 13ᶜ when m = c+1  */

    for (idx_t m = 2; m <= n; ++m)         /* lift to 13^(m+1)  */
    {
        /* New modulus */
        seq_t pow_next = pow * 13;          /* 13^(m+1) */

        /* cube = cur³ (mod pow_next) */
        seq_t k2;
        seq_t cube;
#if defined(__SIZEOF_INT128__)          /* use 128‑bit intermediate when available */
        k2   = (seq_t)(((__uint128_t)cur * cur) % pow_next);
        cube = (seq_t)(((__uint128_t)k2 * cur) % pow_next);
#else                                    /* fallback: rely on seq_t operators (works for big‑int) */
        k2   = cur * cur;
        k2   %= pow_next;
        cube = k2 * cur;
        cube %= pow_next;
#endif

        /* e = ((cur³ - 5) / pow)  (this number is 0 ≤ e < 13) */
        seq_t d = cube - (seq_t)5;
        seq_t e = d / pow;
        idx_t e_mod = (idx_t)(e % 13);      /* e fits in 8 bits, safe to cast */

        /* Compute the Hensel lift digit t.  inv(4) mod 13 is 10. */
        idx_t t = (13 - (e_mod * 10) % 13) % 13;

        /* Update root: cur ← cur + t·13ᶜ  */
        cur += (seq_t)t * pow;
        pow  = pow_next;                   /* advance to next power of 13 */
    }

    return cur;
}

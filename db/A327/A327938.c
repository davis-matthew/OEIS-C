/*
 * A327938  Multiplicative with a(p^e) = p^(e mod p).
 * https://oeis.org/A327938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} p^(e mod p) (multiplicative)
 * Range:     u8 n <= 256, u16 n <= 9277, u32 n <= 9277, u64 n <= 9277,
 *            u128 n <= 9277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The function a(n) computes the value of OEIS sequence A327938,
   multiplicative with a(p^e) = p^(e mod p).  It works for any
   numeric seq_t type (uint8_t, uint16_t, uint32_t, uint64_t,
   unsigned __int128, or big‑int class) and for any idx_t (uint64_t). */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;              /* work in native 64‑bit */
    seq_t   result = (seq_t)1;
    uint64_t p, e, exp_mod;

    /* handle factor 2 separately */
    if ((temp & 1ULL) == 0) {
        e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        exp_mod = e % 2;                     /* 2 divides itself mod 2 */
        seq_t contrib = (seq_t)1;
        for (unsigned int i = 0; i < exp_mod; ++i)
            contrib *= (seq_t)2;
        result *= contrib;
    }

    /* now find odd prime factors up to sqrt(temp) */
    p = 3;
    while (p * p <= temp) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exp_mod = e % p;                 /* p is odd here */
            seq_t contrib = (seq_t)1;
            for (uint64_t i = 0; i < exp_mod; ++i)
                contrib *= (seq_t)p;
            result *= contrib;
        }
        p += 2;                              /* skip even numbers */
    }

    /* if anything remains it is a prime factor > sqrt(original n) */
    if (temp > 1) {
        p = temp;
        e = 1;
        exp_mod = e % p;                     /* p > 1, so mod is 1 */
        seq_t contrib = (seq_t)1;
        for (uint64_t i = 0; i < exp_mod; ++i)
            contrib *= (seq_t)p;
        result *= contrib;
    }

    return result;
}

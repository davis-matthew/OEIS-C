/*
 * A257403  Multiplicative with a(2) = 1, a(2^e) = 0 if e>1, a(3^e) = 0^e,
 *          a(p^e) = e+1 if p == 1, 3 (mod 8), a(p^e) = (1 + (-1)^e) / 2 if p
 *          == 5, 7 (mod 8).
 * https://oeis.org/A257403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with a(1)=1, * a(2)=1, a(2^e)=0 for e>1,
 *            a(3^e)=0, * a(p^e)=e+1 for primes p≡1 or 3 (mod 8), *
 *            a(p^e)=(1+(-1)^e)/2 for primes p≡5 or 7 (mod 8)
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   is_mod13, is_mod57
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_mod13.h"
#include "../utils/is_mod57.h"

/* The sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 1 */
    if (n <= 1) return (seq_t)1;

    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;
    uint64_t p;
    unsigned e;

    /* factor 2 separately */
    if ((tmp & 1ULL) == 0) {
        e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            e++;
        }
        /* a(2^e) */
        if (e > 1) return (seq_t)0;
        /* e==1 -> local value 1, multiplication by 1 does nothing */
    }

    /* factor odd primes */
    for (p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            seq_t lv;
            if (p == 3ULL) {
                lv = 0;
            } else if (is_mod13(p)) {
                lv = (seq_t)(e + 1);
            } else if (is_mod57(p)) {
                lv = (e & 1) ? (seq_t)0 : (seq_t)1;
            } else {
                /* This can only happen for p≡0,2,4,6 mod 8 which cannot be odd prime */
                lv = 1;
            }
            if (lv == 0) return (seq_t)0;
            result *= lv;
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        p = tmp;
        /* exponent is 1 */
        seq_t lv;
        if (p == 3ULL) {
            lv = 0;
        } else if (is_mod13(p)) {
            lv = (seq_t)(1 + 1);
        } else if (is_mod57(p)) {
            lv = 1;   /* e=1 odd -> 0, but here e=1, so check: (1+(-1)^1)/2 = 0 */
            if ((1 & 1) == 0) lv = 1; else lv = 0;
            /* the above computes 0 actually */
            lv = 0;
        } else {
            lv = 1;
        }
        if (lv == 0) return (seq_t)0;
        result *= lv;
    }

    return result;
}

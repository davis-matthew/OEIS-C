/*
 * A349913  Sum of A001227 (the number of odd divisors function) and its
 *          Dirichlet inverse.
 * https://oeis.org/A349913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9966, u16 n <= 9966, u32 n <= 9966, u64 n <= 9966,
 *            u128 n <= 9966, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A349913
 *  a(n) = A001227(n) + A327276(n)
 *  where A001227(n) is the number of odd divisors of n
 *  and A327276 is its Dirichlet inverse.
 *  For n not divisible by 4:
 *      a(n) = d_odd(n) + (-1)^k * ∏_{p^e||n, p odd} g(p^e)
 *  with k = exponent of 2 in n and
 *      g(p)   = -2
 *      g(p^2) =  1
 *      g(p^e) =  0  (e ≥ 3)
 *  For n divisible by 4: a(n) = d_odd(n).
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- split off powers of two ---------- */
    unsigned int k = __builtin_ctzll((uint64_t)n);          /* exponent of 2 */
    uint64_t odd = (uint64_t)(n >> k);                    /* odd part */

    /* ---------- compute odd divisor count d = A001227(n) ---------- */
    uint64_t d = 1;
    uint64_t t = odd;
    /* factor odd part by trial division (n ≤ ~10⁴ → fast) */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            d *= (uint64_t)(e + 1);
        }
    }
    if (t > 1) {     /* remaining prime factor */
        d *= 2;       /* exponent 1 → e+1 = 2 */
    }

    /* ---------- compute Dirichlet inverse part ---------- */
    int64_t g = 1;         /* product over odd prime powers */
    bool zero = false;     /* becomes true if any exponent ≥ 3 */
    t = odd;
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            if (e >= 3) {
                zero = true;
                break;
            }
            if (e == 1) {
                g *= -2;      /* factor for p^1 */
            }               /* factor for p^2 is 1 → nothing to do */
        }
    }
    if (!zero && t > 1) {  /* remaining prime factor (exponent 1) */
        g *= -2;
    }
    if (zero) {
        g = 0;
    }

    /* multiply by 2‑part factor */
    int64_t f;
    if (k >= 2) {
        f = 0;                 /* f(n) = 0 when n divisible by 4 */
    } else if (k == 1) {
        f = -g;                /* n ≡ 2 (mod 4) */
    } else {
        f = g;                 /* n odd */
    }

    /* ---------- final value ---------- */
    int64_t result = (int64_t)d + f;   /* may be negative only when n=1? no */
    return (seq_t)result;
}

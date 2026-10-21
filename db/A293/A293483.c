/*
 * A293483  The number of 6th powers in the multiplicative group modulo n.
 * https://oeis.org/A293483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} f(p^e) where f(2^e) = 1 (e≤3), 2^(e-3) (e≥3)
 *            f(3^e) = 1 (e≤2), 3^(e-2) (e≥2) f(p^e) = φ(p^e)/2 (p≡5 mod 6)
 *            f(p^e) = φ(p^e)/6 (p≡1 mod 6) a(1) = 1
 * Range:     u8 n <= 520, u16 n <= 9652, u32 n <= 9652, u64 n <= 9652,
 *            u128 n <= 9652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer power for seq_t types */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1ULL) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* main function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    uint64_t temp = nn;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            uint64_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            primes[cnt] = d;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {  /* remaining prime factor */
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e = (idx_t)exps[i];
        seq_t part;

        if (p == 2) {
            if (e <= 3) part = 1;
            else part = pow_seq((seq_t)2, e - 3);
        } else if (p == 3) {
            if (e <= 2) part = 1;
            else part = pow_seq((seq_t)3, e - 2);
        } else {
            /* φ(p^e) = p^(e-1)*(p-1) */
            seq_t phi = pow_seq((seq_t)p, e - 1);
            phi *= (p - 1);
            if (p % 6 == 5)
                part = phi / 2;
            else               /* p % 6 == 1 */
                part = phi / 6;
        }
        result *= part;
    }

    return result;
}

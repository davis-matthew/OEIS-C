/*
 * A343292  Number of distinct results produced when generating a graphical
 *          image of each row of the multiplication table modulo n.
 * https://oeis.org/A343292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (φ(n) - A060594(n))/2 where φ is Euler's totient
 *            function and A060594(n) is the number of solutions to x^2 ≡ 1
 *            (mod n)
 * Range:     u8 n <= 298, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses factor_u64 to factor n, then computes φ(n) and the
   2‑adic solution count for x^2 ≡ 1 (mod n). */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factorization arrays – enough for any 64‑bit integer */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Euler's totient φ(n) */
    seq_t phi = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        phi = phi - phi / p;
    }

    /* Count of solutions to x^2 ≡ 1 (mod n)  */
    unsigned k = 0;          /* distinct odd primes */
    uint64_t e2 = 0;         /* exponent of 2 in n */
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2)
            e2 = ex[i];
        else
            ++k;
    }

    seq_t sols;
    if (e2 == 0 || e2 == 1)           /* n odd or n ≡ 2 (mod 4) */
        sols = (seq_t)1 << k;
    else if (e2 == 2)                 /* n divisible by 4 but not 8 */
        sols = (seq_t)1 << (k + 1);
    else                               /* n divisible by 8 or more */
        sols = (seq_t)1 << (k + 2);

    /* a(n) = n - (φ(n) - sols)/2 */
    seq_t delta = phi - sols;
    seq_t half = delta / 2;
    seq_t res = (seq_t)n - half;
    return res;
}

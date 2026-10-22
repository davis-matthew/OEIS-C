/*
 * A360430  Dirichlet convolution of Dedekind psi by A038040.
 * https://oeis.org/A360430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} A001615(n/d)*A000005(d)*d * Multiplicative
 *            form: for n = ∏ p^e, a(n) = ∏ ((e+2)*p+e) * (e+1) * p^(e-1) / 2
 * Range:     u8 n <= 11, u16 n <= 335, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for cast to (seq_t) */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but safe */
    seq_t result = 1;
    idx_t temp = n;
    /* Handle factor 2 separately */
    int cnt = 0;
    while ((temp & 1ULL) == 0) {  /* temp % 2 == 0 */
        temp >>= 1;
        cnt++;
    }
    if (cnt > 0) {
        seq_t term = (seq_t)(cnt + 2) * (seq_t)2 + cnt;   /* (e+2)*p + e */
        term = term * (cnt + 1);                          /* * (e+1) */
        seq_t pow = 1;
        for (int i = 1; i < cnt; i++) pow *= (seq_t)2;   /* 2^(e-1) */
        term = term * pow;
        term = term / 2;                                  /* / 2 */
        result *= term;
    }

    /* Odd primes */
    idx_t p = 3;
    while (p <= temp / p) {  /* avoid overflow on p*p */
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            seq_t term = (seq_t)(e + 2) * (seq_t)p + e;   /* (e+2)*p + e */
            term = term * (e + 1);                       /* * (e+1) */
            seq_t pow = 1;
            for (int i = 1; i < e; i++) pow *= (seq_t)p; /* p^(e-1) */
            term = term * pow;
            term = term / 2;                             /* / 2 */
            result *= term;
        }
        p += 2;
    }

    /* Remaining prime factor if any */
    if (temp > 1) {
        idx_t prime = temp;
        int e = 1;
        seq_t term = (seq_t)(e + 2) * (seq_t)prime + e;  /* (e+2)*p + e */
        term = term * (e + 1);                          /* * (e+1) */
        /* p^(e-1) == 1 for e == 1 */
        term = term / 2;                                /* / 2 */
        result *= term;
    }

    return result;
}

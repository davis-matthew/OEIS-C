/*
 * A259761  Expansion of (phi(x)^2 + phi(x^9)^2) / 2 in powers of x where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A259761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259761: Expansion of (phi(x)^2 + phi(x^9)^2) / 2
   The sequence is defined multiplicatively:
   a(0) = 1
   a(n) = 2 * b(n) for n > 0
   where
     b(2^e) = 1
     b(3^e) = 1 + (-1)^e   (e>0)
     b(p^e) = e+1          if p ≡ 1 or 5 (mod 12)
     b(p^e) = (1 + (-1)^e) / 2   if p ≡ 7 or 11 (mod 12)
   For primes p>3 these congruences cover all cases. */

/* Computes a(n) for n ≥ 0.  Multiplicative factorization is performed
   on the fly; it is fast enough for n up to at least 10^5. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                    /* a(0) = 1 */

    idx_t x = n;
    seq_t prod = 1;                 /* stores b(n) */

    /* factor 2: multiplicative factor is always 1 */
    while ((x & 1) == 0)
        x >>= 1;                    

    /* factor 3 */
    idx_t e3 = 0;
    while ((x % 3) == 0) {           /* x divisible by 3 */
        x /= 3;
        ++e3;
    }
    if (e3 > 0) {
        /* 1 + (-1)^e = 2 if e even, 0 if e odd */
        seq_t factor = (e3 & 1) ? 0 : 2;
        prod *= factor;
        if (prod == 0)
            return 0;               /* early exit */
    }

    /* factor remaining odd primes > 3 */
    for (idx_t p = 5; p * p <= x; p += 2) {
        if (x % p != 0)
            continue;
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }

        seq_t factor;
        if ((p % 12) == 1 || (p % 12) == 5) {
            factor = e + 1;          /* e+1 */
        } else {                    /* p ≡ 7 or 11 (mod 12) */
            factor = (e & 1) ? 0 : 1;/* 0 if e odd, 1 if e even */
        }

        prod *= factor;
        if (prod == 0)
            return 0;
    }

    /* if anything remains, it is a prime > sqrt(original n) */
    if (x > 1) {
        idx_t p = x;
        idx_t e = 1;                 /* exponent is 1 */

        seq_t factor;
        if ((p % 12) == 1 || (p % 12) == 5) {
            factor = 2;            /* e+1 = 2 */
        } else {                    /* p ≡ 7 or 11 (mod 12) */
            factor = 1;            /* e is odd -> factor 0, but e=1 so factor 0? */
            /* For p ≡ 7 or 11, (1 + (-1)^e)/2 = 0 when e is odd */
            factor = 0;            /* actually zero */
        }

        prod *= factor;
        if (prod == 0)
            return 0;
    }

    return prod * (seq_t)2;         /* a(n) = 2 * b(n) */
}

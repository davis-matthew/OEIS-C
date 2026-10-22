/*
 * A376203  a(n) = A376202(2*n-1)/2.
 * https://oeis.org/A376203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A376202(2*n-1)/2 Here A376202(k) is φ(k) * 2^(d-1) where
 *            d = number of distinct prime factors p ≡ 1 (mod 6) of k,
 *            provided that • k has no prime factor ≡ 5 (mod 6), • the prime 3
 *            occurs at most once. Otherwise A376202(k) = 0
 * Range:     u8 n <= 201, u16 n <= 7814, u32 n <= 7814, u64 n <= 7814,
 *            u128 n <= 7814, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements this definition. */

seq_t a(idx_t n)
{
    /* k = 2*n-1 is always odd */
    idx_t k = 2 * n - 1;
    if (k <= 1) return 0;          /* handles n == 1 (k == 1) */

    seq_t phi = 1;                  /* running totient product */
    int dcount = 0;                 /* distinct primes ≡ 1 (mod 6) */

    /* handle factor 3: exponent must be 0 or 1 */
    int e3 = 0;
    while ((k % 3) == 0) {
        e3++;
        if (e3 > 1) return 0;      /* exponent > 1 not allowed */
        phi *= (seq_t)(3 - 1);     /* multiply by 2 for the single 3 factor */
        k /= 3;
    }

    /* trial division for remaining odd factors */
    for (idx_t p = 5; p <= k / p; p += 2) {
        if (k % p == 0) {
            int exp = 0;
            while (k % p == 0) {
                k /= p;
                exp++;
            }

            if (p % 6 == 5) return 0;   /* forbidden prime ≡ 5 (mod 6) */

            if (p % 6 == 1) {
                dcount++;               /* new distinct prime of type 1 mod 6 */

                /* multiply phi by (p-1) * p^(exp-1) */
                phi *= (seq_t)(p - 1);
                seq_t p_pow = 1;
                for (int i = 1; i < exp; ++i) p_pow *= (seq_t)p;
                phi *= p_pow;
            } else {
                /* should not occur for odd primes other than 3 */
                return 0;
            }
        }
    }

    /* leftover prime factor, if any */
    if (k > 1) {
        if (k % 6 == 5) return 0;       /* forbidden prime ≡ 5 (mod 6) */
        if (k % 6 == 1) {
            dcount++;
            phi *= (seq_t)(k - 1);       /* exponent is 1, so p^(exp-1)=1 */
        } else {
            /* leftover is 3 but exponent >1 would have been caught earlier */
            return 0;
        }
    }

    /* compute multiplier 2^(dcount-1), or 1 if dcount==0 */
    seq_t mult = 1;
    if (dcount >= 1) {
        for (int i = 0; i < dcount - 1; ++i) mult <<= 1;
    }

    /* a(n) = A376202(2*n-1)/2 = (phi * mult)/2 */
    seq_t result = phi;
    result /= 2;        /* safe because phi is even for all k > 1 */
    result *= mult;
    return result;
}

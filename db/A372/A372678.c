/*
 * A372678  a(n) = phi(14 * n)/6.
 * https://oeis.org/A372678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = phi(14*n)/6
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a prime power p^e: a(p^e) = p^e if p==2 or p==7, otherwise (p-1)*p^(e-1) */
/* Multiplicative over the prime factorization of n. */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;

    /* Handle factor 2 */
    if (temp % 2 == 0) {
        idx_t e = 0;
        while (temp % 2 == 0) { temp /= 2; ++e; }
        seq_t pow = 1;
        for (idx_t i = 0; i < e; ++i) pow *= 2;
        result *= pow;                /* multiply by 2^e */
    }

    /* Handle factor 7 */
    if (temp % 7 == 0) {
        idx_t e = 0;
        while (temp % 7 == 0) { temp /= 7; ++e; }
        seq_t pow = 1;
        for (idx_t i = 0; i < e; ++i) pow *= 7;
        result *= pow;                /* multiply by 7^e */
    }

    /* Handle remaining odd primes > 1 */
    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            idx_t e = 0;
            while (temp % i == 0) { temp /= i; ++e; }
            seq_t pow = 1;
            /* pow = i^(e-1) */
            for (idx_t j = 0; j + 1 < e; ++j) pow *= i;
            seq_t val = (seq_t)(i - 1) * pow; /* (p-1)*p^(e-1) */
            result *= val;
        }
    }

    /* Remaining prime factor > sqrt(original temp) */
    if (temp > 1) {
        /* exponent is 1, thus val = (p-1) */
        result *= (seq_t)(temp - 1);
    }

    return result;
}

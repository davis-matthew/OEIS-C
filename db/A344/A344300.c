/*
 * A344300  Expansion of Sum_{k>=1} (-1)^(k+1) * k^2 * x^(k^2) / (1 -
 *          x^(k^2)).
 * https://oeis.org/A344300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A344300: 
   a(n) = product over prime powers p^e dividing n of 
   a(p^e) where
      a(2^e) = 2 - (2^(2*floor(e/2)+2) - 1)/3,
      a(p^e) = (p^(2*floor(e/2)+2) - 1)/(p^2 - 1)   (p odd).
   This uses the multiplicativity of the sequence and prime factorisation.
*/

static inline seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;   /* offset starts at 1 */
    }

    idx_t temp = n;
    seq_t result = (seq_t)1;

    /* handle factor 2 separately */
    unsigned e2 = 0;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        e2++;
    }
    if (e2 > 0) {
        unsigned m = 2 * (e2 / 2) + 2;
        seq_t pow2 = power_seq((seq_t)2, m);
        seq_t val = (seq_t)2 - (pow2 - (seq_t)1) / (seq_t)3;
        result *= val;
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            unsigned m = 2 * (exp / 2) + 2;
            seq_t powp = power_seq((seq_t)p, m);
            seq_t denom = (seq_t)p * (seq_t)p - (seq_t)1;
            seq_t val = (powp - (seq_t)1) / denom;
            result *= val;
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {
        unsigned exp = 1;
        unsigned m = 2 * (exp / 2) + 2;
        seq_t powp = power_seq((seq_t)temp, m);
        seq_t denom = (seq_t)temp * (seq_t)temp - (seq_t)1;
        seq_t val = (powp - (seq_t)1) / denom;
        result *= val;
    }

    return result;
}

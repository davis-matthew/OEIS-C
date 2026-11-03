/*
 * A259024  a(4*n) = a(n). a(4*n + 2) = 0. a(4*n + 1) = - A259022(n+2). a(4*n
 *          + 3) = - A259022(n-2).
 * https://oeis.org/A259024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative with a(2^e) = (1 + (-1)^e)/2, a(3) = -1,
 *            a(3^e) = 0 if e>1, a(p^e) = 1 if p ≡ 1 (mod 6), a(p^e) = (-1)^e
 *            if p ≡ 5 (mod 6)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9992 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t mult_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    /* handle factor 2 */
    int exp2 = 0;
    while ((tmp & 1) == 0) {
        exp2++;
        tmp >>= 1;
    }
    if (exp2 & 1) /* odd exponent of 2 gives zero */
        return (seq_t)0;

    /* handle odd prime factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if ((tmp % p) == 0) {
            int exp = 0;
            while ((tmp % p) == 0) {
                tmp /= p;
                exp++;
            }
            if (p == 3) {
                if (exp == 1)
                    result *= (seq_t)-1;
                else
                    return (seq_t)0; /* 3^e with e>1 gives zero */
            } else {
                if (p % 6 == 1) {
                    result *= (seq_t)1;          /* no change */
                } else if (p % 6 == 5) {
                    /* (-1)^exp */
                    if (exp & 1)
                        result *= (seq_t)-1;
                    else
                        result *= (seq_t)1;
                }
            }
            if (result == (seq_t)0)
                return (seq_t)0;
        }
    }

    /* remaining prime factor > sqrt(orig_n) if any */
    if (tmp > 1) {
        /* tmp is a prime */
        if (tmp == 3) {
            result *= (seq_t)-1;
        } else if (tmp % 6 == 1) {
            result *= (seq_t)1;
        } else if (tmp % 6 == 5) {
            result *= (seq_t)-1;   /* exponent 1 is odd */
        }
    }

    return result;
}

seq_t a(idx_t n)
{
    /* Directly use the multiplicative definition. */
    return mult_a(n);
}

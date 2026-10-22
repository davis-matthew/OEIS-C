/*
 * A349343  Dirichlet inverse of A193356, which is defined as n if n is odd, 0
 *          if n is even.
 * https://oeis.org/A349343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 0 if n is even; for odd n, a(n) = (-1)^k * n where k is
 *            the number of distinct odd prime factors of n, provided n is
 *            squarefree. Otherwise a(n)=0
 * Range:     i8 n <= 128, i16 n <= 9953, i32 n <= 9953, i64 n <= 9953,
 *            i128 n <= 9953, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even numbers give zero directly. */
    if ((n & 1ULL) == 0)
        return (seq_t)0;

    seq_t prod = 1;   /* product of distinct odd prime factors */
    int sign = 1;     /* sign (-1)^k */

    idx_t temp = n;

    /* Factor by trial division using only odd primes. */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
                if (exp > 1)   /* not square‑free */
                    return (seq_t)0;
            }
            prod = prod * (seq_t)p;
            sign = -sign;
        }
    }

    /* Any remaining factor is a single odd prime > sqrt(original n). */
    if (temp > 1) {
        prod = prod * (seq_t)temp;
        sign = -sign;
    }

    return (seq_t)sign * prod;
}

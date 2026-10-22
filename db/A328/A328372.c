/*
 * A328372  Expansion of Sum_{k>=1} x^(k^2) / (1 - x^(2*k^2)).
 * https://oeis.org/A328372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = number of odd divisors d of n such that n/d is a square.
 *            Computation is based on the multiplicative property: a(2^e) = 0
 *            if e odd, 1 if e even; a(p^e) = ceil((e+1)/2) for odd prime p,
 *            which equals (e+2)/2 in integer division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case and factor out powers of 2. */
    if (n == 0) return 0;          /* defined only for n >= 1, but safe guard */
    idx_t N = n;
    idx_t e0 = 0;
    while ((N & 1ULL) == 0 && N > 0) {
        N >>= 1;
        e0++;
    }
    /* If the exponent of 2 is odd, no odd divisor satisfies the condition. */
    if (e0 & 1ULL) return 0;

    seq_t result = 1;              /* multiplicative accumulator */

    /* Trial division by odd primes. */
    for (idx_t p = 3; p <= N / p; p += 2) {
        if (N % p == 0) {
            idx_t e = 0;
            do {
                N /= p;
                e++;
            } while (N % p == 0);
            /* Contribution for prime p^e: ceil((e+1)/2) == (e+2)/2. */
            seq_t contrib = ((seq_t)(e + 2)) / 2;
            result *= contrib;
        }
    }

    /* If a prime factor > sqrt(n) remains, its exponent is 1. */
    if (N > 1) {
        /* For exponent 1, contribution is (1+2)/2 == 1. */
        result *= ((seq_t)(1 + 2)) / 2;
    }

    return result;
}

/*
 * A335032  Dirichlet g.f.: zeta(s) * zeta(s-1) * Product_{p prime} (1 + p^(1
 *          - s) - p^(-s)).
 * https://oeis.org/A335032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = multiplicative function with a(p^e) = σ(p^e) + p^e – 1 (σ
 *            = sum of divisors). Equivalent to a(n) = Σ_{d|n} A176345(d)
 * Range:     u8 n <= 41, u16 n <= 2309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external utilities required; simple trial division suffices for the
   expected input sizes (≤ 10000).  The function works for any seq_t and
   idx_t defined by the build. */

seq_t a(idx_t n)
{
    /* a(1) = 1 (empty product) */
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t remaining = n;

    /* Handle factor 2 separately */
    if ((remaining & 1) == 0) {
        idx_t e = 0;
        while ((remaining & 1) == 0) {
            remaining >>= 1;
            ++e;
        }
        seq_t p = (seq_t)2;
        seq_t pow = p;
        seq_t sigma = 1 + pow;   /* sigma = 1 + 2^e */
        for (idx_t j = 1; j < e; ++j) {
            pow *= p;
            sigma += pow;
        }
        seq_t ap = sigma + pow - (seq_t)1;  /* a(2^e) */
        result *= ap;
    }

    /* Odd prime factors */
    for (idx_t i = 3; i * i <= remaining; i += 2) {
        if (remaining % i == 0) {
            idx_t p = i;
            idx_t e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            seq_t sp = (seq_t)p;
            seq_t pow = sp;
            seq_t sigma = 1 + pow;
            for (idx_t j = 1; j < e; ++j) {
                pow *= sp;
                sigma += pow;
            }
            seq_t ap = sigma + pow - (seq_t)1;
            result *= ap;
        }
    }

    /* If a prime factor > sqrt(n) remains */
    if (remaining > 1) {
        seq_t p = (seq_t)remaining;
        /* e = 1, sigma = 1 + p, pow = p, ap = sigma + p - 1 = 2p */
        seq_t ap = (seq_t)2 * p;
        result *= ap;
    }

    return result;
}

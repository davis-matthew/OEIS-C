/*
 * A355038  a(n) = n^2 times the squarefree kernel of n.
 * https://oeis.org/A355038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^2 * rad(n) = product over prime factors p of n of
 *            p^(2*e + 1), where e = exponent of p in n. Equivalent to a(n) =
 *            n^2 * squarefree kernel(n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not part of the sequence, safety guard */

    /* compute n^2 once */
    seq_t nseq = (seq_t)n;
    seq_t res  = nseq * nseq;

    /* factor n by trial division and accumulate the square‑free part */
    idx_t  m   = n;                      /* remaining part to factor */
    seq_t  rad = 1;                      /* product of distinct prime factors */

    /* handle factor 2 separately for efficiency */
    if (m % 2 == 0) {
        rad *= (seq_t)2;
        while (m % 2 == 0) m /= 2;
    }

    /* trial divide by odd numbers up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            rad *= (seq_t)p;             /* include this prime once */
            while (m % p == 0) m /= p;   /* strip all occurrences */
        }
    }

    /* if a prime factor larger than sqrt(n) remains, include it */
    if (m > 1) rad *= (seq_t)m;

    /* finalize the result */
    res *= rad;
    return res;
}

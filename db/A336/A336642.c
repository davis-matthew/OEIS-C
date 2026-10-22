/*
 * A336642  One less than the largest square dividing n: a(n) = A008833(n)-1.
 * https://oeis.org/A336642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008833(n)-1 * A008833(n) is the largest perfect square
 *            that divides n. * Hence a(n) is that square minus one
 * Range:     u8 n <= 288, u16 n <= 8995, u32 n <= 8995, u64 n <= 8995,
 *            u128 n <= 8995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n <= 1 the largest square divisor is 1, so a(n) = 0. */
    if (n <= 1)
        return (seq_t)0;

    idx_t temp = n;           /* Working copy of n for factorisation.   */
    seq_t sqdiv = (seq_t)1;    /* Product of p^(2*floor(e/2)) over all prime factors. */

    /* ----- Handle factor 2 separately ----- */
    unsigned exp = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        exp++;
    }
    if (exp >= 2) {
        unsigned sq = exp / 2;               /* exponent of 2² in the square divisor. */
        seq_t factor = (seq_t)2 * (seq_t)2;  /* 2² = 4. */
        for (unsigned i = 0; i < sq; ++i)
            sqdiv *= factor;
    }

    /* ----- Factorise the odd part ----- */
    for (idx_t p = 3; p <= temp / p; p += 2) {  /* p <= sqrt(temp)   */
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            if (e >= 2) {
                unsigned sq = e / 2;
                seq_t factor = (seq_t)p * (seq_t)p;  /* p².                 */
                for (unsigned i = 0; i < sq; ++i)
                    sqdiv *= factor;
            }
        }
    }

    /* The remaining value of temp is either 1 or a prime with exponent 1,
       which does not contribute to the square divisor. */

    return sqdiv - (seq_t)1;
}

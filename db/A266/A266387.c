/*
 * A266387  Number of orbits of Aut(Z^7) as function of the infinity norm n of
 *          the representative lattice point of the orbit, when the
 *          cardinality of the orbit is equal to 322560.
 * https://oeis.org/A266387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)/120
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 152, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;

    seq_t numerators[5];
    numerators[0] = (seq_t)(n - 1);
    numerators[1] = (seq_t)(n - 2);
    numerators[2] = (seq_t)(n - 3);
    numerators[3] = (seq_t)(n - 4);
    numerators[4] = (seq_t)(n - 5);

    /* Cancel the denominator factors 1,2,3,4,5 with the numerators */
    for (int di = 0; di < 5; ++di)
    {
        seq_t d = (seq_t)(di + 1);   /* denominator value */
        for (int j = 0; j < 5; ++j)
        {
            seq_t g = seq_gcd(numerators[j], d);
            if (g > 1)
            {
                numerators[j] /= g;
                d /= g;
                if (d == 1)
                    break;
            }
        }
    }

    seq_t binom = (seq_t)1;
    for (int k = 0; k < 5; ++k)
        binom *= numerators[k];

    return binom * (seq_t)7;
}

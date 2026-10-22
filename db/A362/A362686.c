/*
 * A362686  Binomial(n+p, n) mod n where p=6.
 * https://oeis.org/A362686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6, n) mod n
 * Range:     u8 n <= 334, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the helper gcd(seq_t,a,b) from "../utils/gcd.h". */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; guard against an accidental
     * n == 0 with a simple return. */
    if (n == 0)
        return 0;

    /* The binomial coefficient C(n+6, n) = (n+1)(n+2)...(n+6) / 6!.
     * We cancel the denominator factors 1..6 from the numerators to
     * keep everything integral in seq_t. */
    seq_t numer[6];
    for (idx_t i = 0; i < 6; ++i)
        numer[i] = (seq_t)n + (i + 1);          /* n+1 .. n+6 */

    const idx_t denom[6] = {1, 2, 3, 4, 5, 6};

    for (idx_t di = 0; di < 6; ++di)
    {
        idx_t d = denom[di];
        if (d == 1)
            continue;

        /* Cancel d with the numerators one by one. */
        for (idx_t ni = 0; ni < 6 && d > 1; ++ni)
        {
            seq_t g = gcd(numer[ni], (seq_t)d);
            if (g > 1)
            {
                numer[ni] /= g;
                d /= (idx_t)g;
            }
        }
        /* After the loop d should be 1 because 6! divides the product. */
    }

    /* Now compute the product modulo n. */
    seq_t result = 1;
    seq_t mod = (seq_t)n;
    for (idx_t i = 0; i < 6; ++i)
        result = (result * numer[i]) % mod;

    return result;
}

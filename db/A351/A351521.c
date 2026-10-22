/*
 * A351521  Dirichlet g.f.: Product_{p prime} (1 + 4*p^(-s)).
 * https://oeis.org/A351521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 4^{ω(n)} if n is squarefree, else a(n) = 0 (a(1)=1)
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t temp = n;
    unsigned cnt = 0;        /* count of distinct prime factors */

    /* handle factor 2 */
    if (temp % 2 == 0) {
        ++cnt;
        temp /= 2;
        if (temp % 2 == 0)   /* square factor detected */
            return (seq_t)0;
    }

    /* trial division by odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            ++cnt;
            temp /= p;
            if (temp % p == 0)   /* square factor detected */
                return (seq_t)0;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
        ++cnt;

    /* compute 4^cnt */
    seq_t result = (seq_t)1;
    for (unsigned i = 0; i < cnt; ++i)
        result *= (seq_t)4;

    return result;
}

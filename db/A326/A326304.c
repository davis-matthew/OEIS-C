/*
 * A326304  Multiplicative with a(p^k) = a(p-1)^k + 1 for any k > 0 and any
 *          prime number p.
 * https://oeis.org/A326304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   Multiplicative with a(p^k) = a(p-1)^k + 1 for primes p, k>0;
 *            a(1)=1
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Trial‑division factorisation.  */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0)
            continue;

        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* Base for the prime power p^e  : a(p-1)  */
        seq_t base = a(p - 1);

        /* Compute a(p-1)^e (in seq_t)  */
        seq_t pow_val = (seq_t)1;
        for (idx_t i = 0; i < e; ++i)
            pow_val *= base;

        /* a(p^e) = a(p-1)^e + 1  */
        seq_t term = pow_val + (seq_t)1;

        /* Multiply into the running product  */
        result *= term;
    }

    /* If something remains, it is a prime factor larger than sqrt(n). */
    if (temp > 1) {
        seq_t base  = a(temp - 1);  /* a(p-1) for the remaining prime p = temp    */
        seq_t term  = base + (seq_t)1; /* e = 1 for a prime power p^1                */
        result *= term;
    }

    return result;
}

/*
 * A341995  a(n) = 1 if the arithmetic derivative (A003415) of n is a prime,
 *          otherwise 0.
 * https://oeis.org/A341995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010051(A003415(n))
 * Range:     u8 n <= 1072, u16 n <= 9800, u32 n <= 9800, u64 n <= 9800,
 *            u128 n <= 9800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1u) return 0;                /* d(0)=0, d(1)=0  → not prime */

    /* Compute arithmetic derivative: d(n) = Σ e_i * n / p_i  */
    seq_t n_val = (seq_t)n;
    seq_t der = 0;
    idx_t temp = n;
    idx_t p = 2;
    while (p * p <= temp) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            der += (seq_t)e * n_val / (seq_t)p;
        }
        ++p;
    }
    if (temp > 1)                 /* remaining prime factor */
        der += n_val / (seq_t)temp;   /* e = 1 */

    /* Check if derivative is prime */
    if (der <= 1) return 0;      /* 0 or 1 is not prime */

    seq_t t = der;
    idx_t i;
    for (i = 2; (seq_t)i * i <= t; ++i) {
        if (t % i == 0) return 0; /* composite */
    }
    return 1;                     /* prime */
}

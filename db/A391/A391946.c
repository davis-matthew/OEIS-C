/*
 * A391946  a(n) = A276085(n) mod 2310, where A276085 is the primorial base
 *          log-function.
 * https://oeis.org/A391946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276085(n) mod 2310 (A276085 is additive with
 *            a(p)=1,2,6,30,210 for p=2,3,5,7,11 respectively, and a(p)=0 for
 *            all other primes)
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n < 1)
        return 0;

    /* Primes whose contribution is non‑zero modulo 2310. */
    static const idx_t primes[5] = {2, 3, 5, 7, 11};
    /* Their associated additive values. */
    static const idx_t prime_vals[5] = {1, 2, 6, 30, 210};

    idx_t tmp = n;
    seq_t sum = 0;       /* accumulate modulo 2310 */

    for (int i = 0; i < 5; ++i) {
        idx_t p = primes[i];
        seq_t val = prime_vals[i];

        while (tmp % p == 0) {
            tmp /= p;
            sum += val;
            /* keep sum in [0,2309] */
            if (sum >= 2310)
                sum -= 2310;
        }
    }
    /* Any remaining prime factor > 11 contributes 0,
       so we can safely ignore it. */
    return sum;
}

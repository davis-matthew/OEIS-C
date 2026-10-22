/*
 * A384934  Consecutive states of the linear congruential pseudo-random number
 *          generator 254*s mod (2^16+1) when started at s=1.
 * https://oeis.org/A384934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 254 * a(n-1) mod 65537 with a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) satisfies a(1)=1 and a(n)=254*a(n-1) (mod 65537).
       Hence a(n) = 254^(n-1) (mod 65537).  We compute the
       modular power by binary exponentiation – O(log n). */
    if (n <= 1)
        return (seq_t)1;

    const unsigned long long MOD = 65537ULL;   /* 2^16 + 1 */
    unsigned long long base = 254ULL;
    unsigned long long res  = 1ULL;
    unsigned long long e = n - 1;              /* exponent */

    while (e > 0) {
        if (e & 1ULL)
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        e >>= 1;
    }
    return (seq_t)res;
}

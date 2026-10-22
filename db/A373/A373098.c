/*
 * A373098  Last digit of n*2^n.
 * https://oeis.org/A373098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = last digit of n*2^n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is uint64_t, seq_t is an unsigned integer type or big‑int.
       The calculation is performed modulo 10, so all intermediates can
       fit in unsigned long long. */
    if (n == 0)
        return (seq_t)0;

    const unsigned long long mod = 10ULL;
    unsigned long long n_mod10 = (unsigned long long)(n % 10ULL);

    /* fast modular exponentiation: compute 2^n mod 10 */
    unsigned long long exp = n;
    unsigned long long base = 2ULL % mod;
    unsigned long long pow2mod = 1ULL;
    while (exp) {
        if (exp & 1ULL)
            pow2mod = (pow2mod * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    unsigned long long result = (n_mod10 * pow2mod) % mod;
    return (seq_t)result;
}

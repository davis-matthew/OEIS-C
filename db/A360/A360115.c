/*
 * A360115  Number of prime factors p of n for which the 2-adic valuation of
 *          p-1 is greater than that of n-1.
 * https://oeis.org/A360115
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of distinct prime divisors p of n such that * the
 *            2‑adic valuation of (p−1) is greater than that of (n−1)
 * Range:     u8 n <= 8384, u16 n <= 8384, u32 n <= 8384, u64 n <= 8384,
 *            u128 n <= 8384, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: count trailing zero bits of a non‑zero 64‑bit integer. */
static int trailing_zeros_uint64(uint64_t v)
{
    int cnt = 0;
    while ((v & 1) == 0) {
        cnt++;
        v >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour below the offset is irrelevant */

    uint64_t m = n;
    int v2_n = trailing_zeros_uint64(n - 1);
    int res = 0;

    /* factorization by trial division, counting distinct primes */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            if (trailing_zeros_uint64(p - 1) > v2_n) res++;
            while (m % p == 0) m /= p;   /* remove all copies of p */
        }
    }
    if (m > 1) {                       /* remaining prime factor > sqrt(n) */
        if (trailing_zeros_uint64(m - 1) > v2_n) res++;
    }

    return (seq_t)res;
}

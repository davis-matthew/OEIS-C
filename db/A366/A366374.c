/*
 * A366374  a(n) = n / gcd(n, A332214(n)), where A332214 is the Mersenne-prime
 *          fixing variant of permutation A163511.
 * https://oeis.org/A366374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, A332214(n)), where A332214(n) = A163511(n)
 *            with * every prime p(k) in its factorization replaced by
 *            g(p(k)): g(p(k)) = 2^k-1 * when 2^k-1 is a (Mersenne) prime,
 *            otherwise the primes p(k) for the remaining * k are sent, in
 *            increasing order, onto the non-Mersenne primes in increas...
 * Range:     u8 n <= 34, u16 n <= 1594, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 106 terms through n = 8120 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A366374_g[64];   /* g(p(k)) for k = 1..64, stored at k-1 */
static int A366374_have = 0;

static int A366374_is_prime_small(uint64_t x)
{
    if (x < 2) return 0;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return 0;
    return 1;
}

static int A366374_is_mersenne_exp(int k)
{
    switch (k) {
        case 2: case 3: case 5: case 7: case 13: case 17:
        case 19: case 31: case 61:
            return 1;
        default:
            return 0;
    }
}

static int A366374_is_mersenne_prime_small(uint64_t q)
{
    /* q < 2^64; Mersenne primes below that */
    return q == 3 || q == 7 || q == 31 || q == 127 || q == 8191 ||
           q == 131071ULL || q == 524287ULL || q == 2147483647ULL ||
           q == 2305843009213693951ULL;
}

static void A366374_init(void)
{
    if (A366374_have) return;
    /* list of non-Mersenne primes in increasing order (need at most 64) */
    uint64_t nm[64];
    int cnm = 0;
    uint64_t x = 2;
    while (cnm < 64) {
        if (A366374_is_prime_small(x) && !A366374_is_mersenne_prime_small(x))
            nm[cnm++] = x;
        x++;
    }
    int j = 0;
    for (int k = 1; k <= 64; k++) {
        if (A366374_is_mersenne_exp(k))
            A366374_g[k - 1] = (k == 64) ? 0 : (((uint64_t)1 << k) - 1);
        else
            A366374_g[k - 1] = nm[j++];
    }
    A366374_have = 1;
}

static seq_t A366374_A332214(idx_t n)
{
    A366374_init();
    if (n == 0) return (seq_t)1;
    idx_t pos[64];
    int cnt = 0;
    idx_t bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        ++bitpos;
        tmp >>= 1;
    }
    seq_t result = (seq_t)A366374_g[cnt - 1];
    idx_t prev = 0;
    for (int i = 0; i < cnt; ++i) {
        idx_t curr = pos[i];
        idx_t e = curr - prev - 1;
        seq_t base = (seq_t)A366374_g[i];
        for (idx_t k = 0; k < e; ++k) result *= base;
        prev = curr;
    }
    return result;
}

static seq_t A366374_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t x = A366374_A332214(n);
    seq_t g = A366374_gcd((seq_t)n, x);
    return (seq_t)n / g;
}

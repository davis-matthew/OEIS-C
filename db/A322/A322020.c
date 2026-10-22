/*
 * A322020  a(n) = Product_{d|n, gcd(d,n/d) is a prime power} gcd(d,n/d).
 * https://oeis.org/A322020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{d|n, gcd(d,n/d) is a prime power} gcd(d,n/d)
 * Range:     u8 n <= 47, u16 n <= 191, u32 n <= 767, u64 n <= 3839,
 *            u128 n <= 8294, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, enumerates all its divisors recursively
   and multiplies the gcd(d,n/d) whenever that gcd is a prime power.
   The prime‑power test is a small trial division routine that works for
   the expected size of n (≤ 10 000 in the tests). The algorithm runs
   in O(τ(n)) time and uses only integer arithmetic. */

#include <stdint.h>
#include <stdbool.h>

/* ---------- Small helpers ---------- */

/* Euclidean gcd for uint64_t. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Test whether x is 1 or a prime power (including primes). */
static bool is_prime_power(uint64_t x)
{
    if (x <= 1) return true;          /* 1 is treated as a prime power */
    uint64_t p = 0;
    for (uint64_t i = 2; i * i <= x; ++i) {
        if (x % i == 0) { p = i; break; }
    }
    if (p == 0) return true;          /* x itself is prime */
    while (x % p == 0) x /= p;
    return x == 1;
}

/* ---------- Recursively enumerate divisors and accumulate product ---------- */

/*
 * d      : current divisor being built
 * idx    : current prime index in the factorization
 * tot    : total number of distinct primes
 * exps   : exponents of the primes in the factorization of n
 * primes : the distinct primes dividing n
 * n      : the original number
 * prod   : pointer to running product (uint64_t)
 */
static void dfs_div(uint64_t d,
                    int idx,
                    int tot,
                    const int *exps,
                    const uint64_t *primes,
                    uint64_t n,
                    uint64_t *prod)
{
    if (idx == tot) {
        uint64_t g = gcd_u64(d, n / d);
        if (is_prime_power(g))
            *prod *= g;
        return;
    }

    uint64_t p = primes[idx];
    uint64_t pow = 1;   /* p^e */
    for (int e = 0; e <= exps[idx]; ++e) {
        dfs_div(d * pow, idx + 1, tot, exps, primes,
                n, prod);
        pow *= p;
    }
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* safety for offset */

    /* Factor n into primes and exponents */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[12];
    int exps[12];
    int tot = 0;

    for (uint64_t p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            int cnt = 0;
            while (nn % p == 0) { nn /= p; ++cnt; }
            primes[tot] = p;
            exps[tot]   = cnt;
            ++tot;
        }
    }
    if (nn > 1) {          /* remaining prime factor */
        primes[tot] = nn;
        exps[tot]   = 1;
        ++tot;
    }

    uint64_t product = 1;
    dfs_div(1, 0, tot, exps, primes, (uint64_t)n, &product);

    return (seq_t)product;
}

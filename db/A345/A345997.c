/*
 * A345997  Let m = A344005(n) be the smallest number such that n|m*(m+1); let
 *          X = A345992(n) = gcd(n,m); Y = A345993(n) = gcd(n,m+1). Sequence
 *          lists n such that neither X nor Y is equal to n/p^k, where p =
 *          largest prime divisor of n and k is its exponent in n.
 * https://oeis.org/A345997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer n such that neither gcd(n, A344005(n))
 *            nor gcd(n, A344005(n)+1) equals n/p^k, where p is the largest
 *            prime divisor of n and k its exponent in n. This is the
 *            definition of OEIS sequence A345997
 * Range:     u8 n <= 21, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

static uint64_t primes[78498];
static size_t  primes_cnt = 0;

/* ---- prime sieve up to 1,000,000 ------------------------------------- */
static void init_primes(void)
{
    if (primes_cnt > 0) return;
    const int LIMIT = 1000000;
    bool *comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (int i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            primes[primes_cnt++] = (uint64_t)i;
            if ((int64_t)i * i <= LIMIT)
                for (int j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;
        }
    }
    free(comp);
}

/* ---- modular inverse (extended Euclid) for uint64_t ------------------- */
static uint64_t modinv(uint64_t a, uint64_t mod)
{
    int64_t t  = 0, newt = 1;
    int64_t r  = (int64_t)mod, newr = (int64_t)(a % mod);
    while (newr != 0) {
        uint64_t q = r / newr;
        int64_t tmp;
        tmp = t - (int64_t)q * newt;  t = newt;  newt = tmp;
        tmp = r - (int64_t)q * newr;  r = newr;  newr = tmp;
    }
    if (r > 1) return 0;          /* not invertible */
    if (t < 0) t += mod;
    return (uint64_t)t;
}

/* ---- test whether a given integer n satisfies the condition --------- */
static bool is_term(seq_t n)
{
    uint64_t u = (uint64_t)n;          /* n fits in 64‑bits for the tests */
    if (u < 60) return false;         /* first term in the list is 60 */

    /* factorization */
    uint64_t p[16];
    int    e[16];
    size_t cnt = 0;
    init_primes();
    uint64_t tmp = u;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint64_t pr = primes[i];
        if (tmp % pr == 0) {
            int exp = 0;
            while (tmp % pr == 0) { tmp /= pr; ++exp; }
            p[cnt] = pr;  e[cnt] = exp;  ++cnt;
        }
    }
    if (tmp > 1) { p[cnt] = tmp; e[cnt] = 1; ++cnt; }

    if (cnt < 3) return false;        /* needs at least three distinct primes */

    /* largest prime factor and its exponent */
    uint64_t largest_p = p[0];
    int      largest_k = e[0];
    for (size_t i = 1; i < cnt; ++i)
        if (p[i] > largest_p) { largest_p = p[i]; largest_k = e[i]; }

    /* compute n / p^k  (L) */
    uint64_t pk_val = 1;
    for (int i = 0; i < largest_k; ++i) pk_val *= largest_p;
    seq_t L = n / (seq_t)pk_val;

    /* precompute prime^exp for each distinct prime (controls subsets) */
    uint64_t pk[16];
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t val = 1;
        for (int j = 0; j < e[i]; ++j) val *= p[i];
        pk[i] = val;
    }

    /* compute A344005(n) by enumerating all splits of prime powers */
    uint64_t m_min = UINT64_MAX;
    uint64_t masks = 1ULL << cnt;
    for (uint64_t mask = 0; mask < masks; ++mask) {
        uint64_t a = 1;
        for (size_t i = 0; i < cnt; ++i)
            if (mask & (1ULL << i)) a *= pk[i];
        uint64_t b = u / a;
        if (b == 1) {                      /* m ≡ 0 (mod a) with b=1 gives m = u */
            if (u < m_min) m_min = u;
            continue;
        }
        uint64_t candidate;
        if (a == 1)       candidate = u - 1;        /* m ≡ -1 (mod u) */
        else {
            uint64_t inv = modinv(a % b, b);
            if (inv == 0) continue;
            uint64_t k = ((b - 1) % b) * inv % b;
            candidate = a * k;                      /* m < u guaranteed */
        }
        if (candidate > 0 && candidate < m_min) m_min = candidate;
    }

    if (m_min == UINT64_MAX) return false;  /* safety check */

    seq_t m      = (seq_t)m_min;
    seq_t m_plus = m + (seq_t)1;
    seq_t X = gcd(n, m);
    seq_t Y = gcd(n, m_plus);

    if (X == L || Y == L) return false;
    return true;
}

/* ---- the sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for index 0, return 0 */
    idx_t found = 0;
    seq_t x = 1;
    while (found < n) {
        if (is_term(x)) { ++found; if (found == n) return x; }
        ++x;
    }
    return 0;  /* unreachable */
}

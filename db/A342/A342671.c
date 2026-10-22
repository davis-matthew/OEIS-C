/*
 * A342671  a(n) = gcd(sigma(n), A003961(n)), where A003961 is fully
 *          multiplicative with a(prime(k)) = prime(k+1), and sigma is the sum
 *          of divisors of n.
 * https://oeis.org/A342671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 9955, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = gcd(σ(n), A003961(n))
 *
 * σ(n)   : sum of divisors of n
 * A003961: multiplicative function with A003961(p) = next_prime(p) for prime p
 *
 * The function computes the prime factorisation of n, accumulates
 * σ(n) and A003961(n) and returns their greatest common divisor.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime table (enough to factor numbers up to about 10 000)           */
/* ------------------------------------------------------------------ */

#define PRIME_TABLE_LIMIT 200000U          /* generate primes up to 200 000 */
#define MAX_PRIMES          18000U         /* number of primes up to 200 000 */

static uint32_t primes[MAX_PRIMES];
static size_t   prime_cnt = 0;
static bool     primes_inited = false;

static void init_prime_table(void)
{
    if (primes_inited) return;
    bool mark[PRIME_TABLE_LIMIT + 1] = { false };
    for (uint32_t i = 2; i <= PRIME_TABLE_LIMIT; ++i) {
        if (!mark[i]) {
            if (prime_cnt < MAX_PRIMES) primes[prime_cnt++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_TABLE_LIMIT; j += i)
                mark[(size_t)j] = true;
        }
    }
    primes_inited = true;
}

/* ------------------------------------------------------------------ */
/* Return the next prime after a given prime p (p is guaranteed to be\n * a prime in primes[])                                     */
/* ------------------------------------------------------------------ */

static uint32_t next_prime_of(uint32_t p)
{
    /* binary search to find index of p */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    /* p is at index lo; next prime is lo+1 */
    if (lo + 1 < prime_cnt) return primes[lo + 1];
    /* fallback: search beyond the table (unlikely for n ≤ 10 000) */
    uint32_t q = p + 1;
    while (true) {
        bool is_prime = true;
        for (size_t k = 0; k < prime_cnt && (uint64_t)primes[k] * primes[k] <= q; ++k) {
            if (q % primes[k] == 0) { is_prime = false; break; }
        }
        if (is_prime) return q;
        ++q;
    }
}

/* ------------------------------------------------------------------ */
/* Euclidean gcd for seq_t                                            */
/* ------------------------------------------------------------------ */

static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_prime_table();

    seq_t sigma      = (seq_t)1;
    seq_t a003961    = (seq_t)1;

    idx_t m = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= (uint64_t)m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            /* determine exponent e */
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* σ part: (p^{e+1} – 1)/(p – 1) */
            seq_t p_pow = (seq_t)1;
            for (uint32_t k = 0; k < e; ++k) p_pow *= (seq_t)p;
            seq_t p_pow_plus1 = p_pow * (seq_t)p;               /* p^{e+1} */
            seq_t sigma_part = (p_pow_plus1 - (seq_t)1) / ((seq_t)p - (seq_t)1);
            sigma *= sigma_part;

            /* A003961 part: next_prime(p)^{e} */
            uint32_t q = next_prime_of(p);
            seq_t q_pow = (seq_t)1;
            for (uint32_t k = 0; k < e; ++k) q_pow *= (seq_t)q;
            a003961 *= q_pow;
        }
    }

    /* remaining prime factor > sqrt(n) (if any) */
    if (m > 1) {
        uint32_t p = (uint32_t)m;        /* exponent is 1 */
        seq_t sigma_part = (seq_t)p + (seq_t)1;    /* p^2-1 / (p-1) = p+1 */
        sigma *= sigma_part;

        uint32_t q = next_prime_of(p);
        a003961 *= (seq_t)q;
    }

    return seq_gcd(sigma, a003961);
}

/*
 * A348172  a(n) is the number of positive k (can be greater than n) such that
 *          A000005(n)/n = A000005(k)/k.
 * https://oeis.org/A348172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k ≥ 1 : τ(n)/n = τ(k)/k}| (τ = divisor count)
 * Range:     u8 n <= 20, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Greatest common divisor for idx_t --------------------------------- */
static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Simple prime sieve up to 20000 ------------------------------------- */
#define SIEVE_MAX 20000
static int prime_list[SIEVE_MAX + 1];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0) return;          /* already done */
    bool is_composite[SIEVE_MAX + 1] = {false};
    for (int p = 2; p <= SIEVE_MAX; ++p) {
        if (!is_composite[p]) {
            prime_list[prime_cnt++] = p;
            if ((int64_t)p * p <= SIEVE_MAX) {
                for (int q = p * p; q <= SIEVE_MAX; q += p)
                    is_composite[q] = true;
            }
        }
    }
}

/* Factorise x (x ≤ 4·10⁸) using the prime list ----------------------- */
static void factorize_small(uint64_t x, int primes_out[20], int exps_out[20], size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        int p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > x) break;
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            primes_out[(*cnt)] = p;
            exps_out[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (x > 1) {
        primes_out[(*cnt)] = (int)x;
        exps_out[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* Compute τ(n) ------------------------------------------------------- */
static seq_t tau_of_uint64(uint64_t n)
{
    int primes[20], exps[20];
    size_t cnt;
    factorize_small(n, primes, exps, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result = result * (seq_t)(exps[i] + 1);
    return result;
}

/* Main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, just guard */
    init_primes();

    /* τ(n) and reduced fraction p/q = τ(n)/n */
    seq_t tau_n = tau_of_uint64((uint64_t)n);
    idx_t g = gcd_u64((idx_t)tau_n, n);
    seq_t p = tau_n / (seq_t)g;    /* numerator */
    idx_t q = n / g;               /* denominator */

    /* Upper bound for t in k = q·t : 4·q (loose but safe) */
    uint64_t t_max = 4ull * q;

    seq_t count = 0;

    for (uint64_t t = 1; t <= t_max; ++t) {
        uint64_t k = (uint64_t)q * t;
        seq_t tau_k = tau_of_uint64(k);
        seq_t lhs   = p * (seq_t)t;   /* p·t */
        if (tau_k == lhs)
            ++count;
    }
    return count;
}

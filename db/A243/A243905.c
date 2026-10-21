/*
 * A243905  Multiplicative order of 2 modulo prime(n)^2 for n >= 2.
 * https://oeis.org/A243905
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p * ord_2(p) if p is not a Wieferich prime, * = ord_2(p)
 *            if p is a Wieferich prime, * where p = prime(n) and ord_2(p) is
 *            the multiplicative order of 2 * modulo p. The order is computed
 *            by factoring p – 1 and reducing * the candidate exponent with
 *            repeated tests. A static prime list * is ge...
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 6543, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- static prime table (sieve up to LIMIT) ---------- */
#define LIMIT 200000          /* 200 000 > 10 000th prime (104 729) */

static unsigned char is_prime_arr[LIMIT + 1];
static uint32_t primes_arr[LIMIT];
static size_t   primes_cnt = 0;
static bool     sieve_done = false;

/* Generate the prime list once */
static void
init_primes(void)
{
    if (sieve_done)
        return;
    for (int i = 2; i <= LIMIT; ++i)
        is_prime_arr[i] = 1;
    for (int i = 2; i * i <= LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime_arr[j] = 0;
        }
    }
    for (int i = 2; i <= LIMIT; ++i)
        if (is_prime_arr[i]) {
            primes_arr[primes_cnt++] = (uint32_t)i;
        }
    sieve_done = true;
}

/* Return the k‑th prime (1‑based).  n ≥ 2 ensures k ≥ 2. */
static inline uint32_t
get_prime_k(idx_t k)
{
    init_primes();
    return primes_arr[k - 1];
}

/* Modular exponentiation: (base^exp) mod mod, all 64‑bit values. */
static inline uint64_t
pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return (uint64_t)result;
}

/* multiplicative order of 2 modulo p (p is odd prime) */
static inline uint64_t
order_of_2_mod_p(uint64_t p)
{
    uint64_t d = p - 1;      /* candidate order */
    uint64_t m = d;          /* will be used for factorisation */

    init_primes();
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes_arr[i] * primes_arr[i] <= m; ++i) {
        uint32_t q = primes_arr[i];
        if (m % q != 0U)
            continue;
        /* remove all powers of q from m */
        while (m % q == 0U)
            m /= q;
        /* reduce d by q while possible */
        while (d % q == 0U) {
            uint64_t cand = d / q;
            if (pow_mod_uint64(2, cand, p) == 1U)
                d = cand;
            else
                break;
        }
    }
    /* remaining prime factor > sqrt(p-1), if any */
    if (m > 1U) {
        uint64_t q = m;
        while (d % q == 0U) {
            uint64_t cand = d / q;
            if (pow_mod_uint64(2, cand, p) == 1U)
                d = cand;
            else
                break;
        }
    }
    return d;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;                          /* offset starts at 2 */

    /* prime at index n (1‑based) */
    uint64_t p = (uint64_t)get_prime_k(n);

    /* order of 2 modulo p */
    uint64_t d = order_of_2_mod_p(p);

    /* check for Wieferich prime: 2^(p-1) ≡ 1 (mod p^2) */
    uint64_t mod = (unsigned __int128)p * p;
    bool is_wieferich = (pow_mod_uint64(2, p - 1, mod) == 1U);

    /* return the appropriate value */
    if (is_wieferich)
        return (seq_t)d;
    else
        return (seq_t)(d * p);
}

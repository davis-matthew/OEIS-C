/*
 * A272334  Square root of the largest square dividing 2^n - 1.
 * https://oeis.org/A272334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt( largest square dividing 2^n - 1 ) Computation: a(n)
 *            = product over odd primes p <= n with ord_p(2) | n. For each
 *            such p let v = multiplicity of p in n. If p is a base‑2
 *            Wieferich prime (1093,3511) add one more to the exponent. Then
 *            exponent of p in the square root is floor((v + (p is...
 * Range:     u8 n <= 363, u16 n <= 1091, u32 n <= 1206, u64 n <= 1206,
 *            u128 n <= 1206, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* --- Helper for modular exponentiation (uint64_t) --------------------------------- */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* --- Compute multiplicative order of 2 modulo p (p odd prime) --------------------- */
static uint64_t multiplicative_order(uint64_t p) {
    uint64_t phi = p - 1;        /* p is prime */
    uint64_t order = phi;

    /* factor phi */
    uint64_t temp = phi;
    for (uint64_t q = 2; q * q <= temp; ++q) {
        if (temp % q == 0) {
            /* divide out all q from temp */
            while (temp % q == 0) temp /= q;
            /* try to reduce order by prime factor q */
            while (order % q == 0 && mod_pow_u64(2, order / q, p) == 1ULL)
                order /= q;
        }
    }
    /* remaining prime factor > sqrt(temp) */
    if (temp > 1) {
        uint64_t q = temp;
        while (order % q == 0 && mod_pow_u64(2, order / q, p) == 1ULL)
            order /= q;
    }
    return order;
}

/* --- Power for seq_t ----------------------------------------------------------- */
static seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* --- Global data for primes ---------------------------------------------- */
#define MAX_PRIME 20000               /* covers n up to about 10000 safely */

static bool primes_init = false;
static int primes[MAX_PRIME];
static int prime_cnt = 0;
static uint64_t orders[MAX_PRIME + 1];
static bool wieferich[MAX_PRIME + 1];

/* --- Initialize primes, orders and Wieferich status --------------------------- */
static void init_primes(void) {
    if (primes_init) return;
    /* Sieve */
    bool is_composite[MAX_PRIME + 1] = {false};
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                is_composite[j] = true;
        }
    }
    for (int i = 3; i <= MAX_PRIME; i += 2) {   /* odd primes only */
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            orders[i] = multiplicative_order((uint64_t)i);
            wieferich[i] = (i == 1093 || i == 3511);   /* known base‑2 Wieferich primes */
        }
    }
    primes_init = true;
}

/* --- Main function ------------------------------------------------------------ */
seq_t a(idx_t n) {
    init_primes();
    if (n < 1) return 1;      /* defined only for n >= 1, but safe guard */

    seq_t result = 1;
    for (int idx = 0; idx < prime_cnt && primes[idx] <= (int)n; ++idx) {
        int p = primes[idx];
        /* p must divide 2^n-1: ord_p(2) must divide n */
        uint64_t ord = orders[p];
        if (n % ord != 0) continue;

        /* multiplicity of p in n */
        uint64_t v = 0;
        idx_t temp = n;
        while (temp % p == 0) {
            temp /= p;
            v++;
        }

        /* exponent of p in 2^n-1: 1 (or 2 for Wieferich) + v */
        uint64_t e = (wieferich[p] ? 2ULL : 1ULL) + v;
        uint64_t half = e >> 1;            /* floor(e/2) */

        if (half == 0) continue;
        result *= pow_seq((seq_t)p, half);
    }
    return result;
}

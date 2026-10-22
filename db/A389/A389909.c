/*
 * A389909  a(n) = 2*A064989(n) for odd n, a(n) = A003961(n/2) for even n,
 *          where A003961 is fully multiplicative with a(p) = nextprime(p),
 *          and A064989 is its left inverse.
 * https://oeis.org/A389909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A064989(n) for odd n, * a(n) = A003961(n/2) for even n.
 *            * * A003961(m) = ∏ nextPrime(p)^e over p^e || m * A064989(m) = ∏
 *            prevPrime(p)^e over p^e || m * (prevPrime(3)=2, nextPrime(2)=3,
 *            etc.) * * The implementation uses simple trial‑division for
 *            factorisation * and prime testing. All arit...
 * Range:     u8 n <= 95, u16 n <= 3071, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ==== Small helpers ==== */
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

static uint64_t next_prime(uint64_t n) {
    uint64_t p = n + 1;
    while (!is_prime(p)) ++p;
    return p;
}

static uint64_t prev_prime(uint64_t n) {
    uint64_t p = n - 1;
    while (!is_prime(p)) --p;
    return p;
}

/* Factorises m and returns A003961(m) */
static seq_t mul_next_prime_factors(uint64_t m) {
    seq_t res = 1;
    uint64_t temp = m;

    /* factor 2 separately */
    if ((temp & 1ULL) == 0) {
        uint64_t e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        uint64_t np = next_prime(2);
        seq_t mult = 1;
        for (uint64_t i = 0; i < e; ++i) mult *= (seq_t)np;
        res *= mult;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            uint64_t np = next_prime(p);
            seq_t mult = 1;
            for (uint64_t i = 0; i < e; ++i) mult *= (seq_t)np;
            res *= mult;
        }
    }

    /* remaining prime > sqrt */
    if (temp > 1) {
        uint64_t np = next_prime(temp);
        res *= (seq_t)np;
    }

    return res;
}

/* Factorises m and returns A064989(m)  (m is odd) */
static seq_t mul_prev_prime_factors(uint64_t m) {
    seq_t res = 1;
    uint64_t temp = m;

    /* odd factors only */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            uint64_t pp = prev_prime(p);
            seq_t mult = 1;
            for (uint64_t i = 0; i < e; ++i) mult *= (seq_t)pp;
            res *= mult;
        }
    }

    /* remaining prime > sqrt */
    if (temp > 1) {
        uint64_t pp = prev_prime(temp);
        res *= (seq_t)pp;
    }

    return res;
}

/* ==== Main function ==== */
seq_t a(idx_t n) {
    if ((n & 1ULL) == 0) {                /* even n */
        uint64_t m = n >> 1;              /* m = n/2 */
        return mul_next_prime_factors(m);
    } else {                               /* odd n */
        uint64_t m = (uint64_t)n;          /* m = n (odd) */
        seq_t val = mul_prev_prime_factors(m);
        return val * 2;
    }
}

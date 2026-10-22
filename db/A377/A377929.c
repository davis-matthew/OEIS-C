/*
 * A377929  Quasi-practical numbers: positive integers m such that every k <=
 *          sigma(m)-m is a sum of distinct proper divisors of m.
 * https://oeis.org/A377929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A quasi-practical number is either a prime or a practical number.
 *  Practical numbers can be tested using the prime-factor criterion.
 *  This routine generates the ascending union of the two sorted lists,
 *  skeletonising only what is needed for 10 000-th term or less. */
#include <stdint.h>
#include <stdbool.h>

/* ---------- helper: primality test (trial division) ---------- */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* ---------- helper: practicality test on uint64_t ---------- */
static bool is_practical(uint64_t n)
{
    if (n == 1) return true;
    if (n % 2) return false;          /* odd numbers (except 1) are not practical */

    /* factor 2^e */
    unsigned e = 0;
    uint64_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e;
    }
    if (e == 0) return false;         /* should never happen because n even */
    uint64_t sigma_partial = (1ULL << (e + 1)) - 1;  /* sum of 2^0 … 2^e */

    /* now test the remaining odd prime factors */
    uint64_t p = 3;
    while (p * p <= tmp) {
        if (tmp % p == 0) {
            unsigned f = 0;
            uint64_t p_pow = 1;
            while (tmp % p == 0) {
                tmp /= p;
                ++f;
                p_pow *= p;   /* p^f */
            }

            /* criterion : p <= sigma_partial + 1 */
            if (p > sigma_partial + 1) return false;

            /* update sigma_partial */
            uint64_t num = 1;
            for (unsigned i = 0; i < f + 1; ++i) num *= p;  /* p^(f+1) */
            num -= 1;
            uint64_t denom = p - 1;                     /* p-1 divides num */
            sigma_partial *= num / denom;
        }
        p += 2;
    }

    /* remaining prime factor if any */
    if (tmp > 1) {
        uint64_t p_rem = tmp;
        if (p_rem > sigma_partial + 1) return false;
        /* exponent 1 */
        uint64_t num = p_rem * p_rem - 1;   /* p^2 - 1 */
        uint64_t denom = p_rem - 1;
        sigma_partial *= num / denom;
    }

    return true;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    /* index n (>=1) -> n-th quasi‑practical number */
    uint64_t count = 0;       /* number of quasi‑practical numbers found */
    for (uint64_t m = 1; ; ++m) {
        bool good = false;
        if (m == 1) {
            good = true;          /* 1 is practical */
        } else if (is_prime(m)) {
            good = true;          /* primes are quasi‑practical */
        } else if ((m & 1ULL) == 0 && is_practical(m)) {
            good = true;          /* even practical numbers */
        }

        if (good) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}

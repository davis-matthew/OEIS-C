/*
 * A324706  The sum of the tri-unitary divisors of n.
 * https://oeis.org/A324706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum of the tri‑unitary divisors of n. (Multiplicative:
 *            a(p^e) = 1+p+…+p^e except a(p^3) = 1+p+p^2+p^3, a(p^6) =
 *            1+p^2+p^4+p^6.) The implementation uses the multiplicative
 *            formula and a simple prime sieve. It works for any seq_t of size
 *            8 or greater
 * Range:     u8 n <= 119, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---- integer square root (floor) for uint64_t ---- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ---- prime table cache ---- */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static uint64_t prime_limit   = 0;

/* Ensure that all primes ≤ limit are stored in prime_list */
static void _ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;

    size_t sz = (size_t)limit + 1;
    unsigned char *is_p = (unsigned char *)calloc(sz, 1);
    if (!is_p) exit(1);
    for (size_t i = 2; i < sz; ++i) is_p[i] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_p[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_p[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_p[i]) ++cnt;

    /* Allocate/replace list */
    uint64_t *list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!list) exit(1);
    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_p[i]) list[k++] = i;
    free(is_p);
    if (prime_list) free(prime_list);
    prime_list = list;
    prime_count = cnt;
    prime_limit = limit;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of bounds, safe return */

    uint64_t temp = (uint64_t)n;
    uint64_t res128 = 1;                /* 128‑bit accumulator */
    uint64_t limit   = isqrt_u64(temp);
    _ensure_primes(limit);

    for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* compute a(p^e) */
            uint64_t factor;
            if (e == 3) {
                uint64_t p2 = p * p;
                uint64_t p3 = p2 * p;
                factor = 1 + p + p2 + p3;
            } else if (e == 6) {
                uint64_t p2 = p * p;
                uint64_t p4 = p2 * p2;
                uint64_t p6 = p4 * p2;
                factor = 1 + p2 + p4 + p6;
            } else {
                uint64_t power = 1;
                for (idx_t j = 0; j < e; ++j) power *= p;
                factor = 1 + power;
            }

            res128 = (unsigned __int128)res128 * factor;
        }
    }

    if (temp > 1) {      /* remaining prime factor (exponent 1) */
        uint64_t factor = 1 + temp;
        res128 = (unsigned __int128)res128 * factor;
    }

    /* cast to seq_t – in 64‑bit builds this fits where required */
    return (seq_t)res128;
}

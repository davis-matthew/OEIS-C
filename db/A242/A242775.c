/*
 * A242775  Let b_k=3...3 consist of k>=1 3's. Then a(n) is the smallest k
 *          such that the concatenation b_k and prime(n) is prime, or a(n)=0
 *          if there is no such prime.
 * https://oeis.org/A242775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 237, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   digits_of, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest k such that the decimal concatenation of
 * b_k (k digits all 3) and prime(n) is prime, or 0 if no such k exists.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/digits_of.h" /* int digits_of(idx_t x); */

/* 128‑bit modular multiplication (adds twice per loop). */
static __uint128_t mulmod128(__uint128_t a, __uint128_t b, __uint128_t mod)
{
    __uint128_t res = 0;
    while (b) {
        if (b & 1)
            res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}

/* 128‑bit modular exponentiation. */
static __uint128_t powmod128(__uint128_t a, __uint128_t e, __uint128_t mod)
{
    __uint128_t res = 1;
    while (e) {
        if (e & 1)
            res = mulmod128(res, a, mod);
        a = mulmod128(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin for 128‑bit integers.
 * The bases {2,3,5,7,11,13,17,19,23,29,31,37} are sufficient for all numbers
 * encountered in this sequence (practical guarantee). */
static bool is_prime_uint128(__uint128_t n)
{
    if (n < 2)
        return false;

    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };

    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* express n-1 as d·2^s */
    __uint128_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t a64 = small[i];
        if (a64 % n == 0)
            continue;
        __uint128_t a = a64;
        __uint128_t x = powmod128(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mulmod128(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;          /* as per the OEIS definition */

    uint32_t prime = get_prime(n);    /* n‑th prime (1‑based) */
    int d = digits_of((idx_t)prime);  /* number of decimal digits of the prime */

    /* compute 10^d in 128‑bit */
    __uint128_t pow10 = 1;
    for (int i = 0; i < d; ++i)
        pow10 *= 10;

    __uint128_t b = 0;                     /* current block of k 3’s (128‑bit) */
    for (idx_t k = 1; ; ++k) {
        b = b * 10 + 3;                     /* build b_k */
        __uint128_t cand = b * pow10 + prime;  /* concatenate b_k and prime */

        if (is_prime_uint128(cand))
            return (seq_t)k;
    }
}

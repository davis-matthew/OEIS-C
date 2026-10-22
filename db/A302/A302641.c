/*
 * A302641  Number of nonnegative integers k such that n^2 - 3*2^k can be
 *          written as x^2 + 2*y^2 with x and y integers.
 * https://oeis.org/A302641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integers k such that * n^2 - 3*2^k
 *            can be expressed as x^2 + 2*y^2 for integers x, y
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  utility functions for 128‑bit arithmetic  ---------- */

/* multiply a * b modulo mod using double‑and‑add to avoid overflow */
static __int128 mul_mod128(__int128 a, __int128 b, __int128 mod)
{
    __int128 res = 0;
    a %= mod;
    while (b) {
        if (b & 1)
            res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}

/* power modulo: base^exp mod mod */
static __int128 pow_mod128(__int128 base, __int128 exp, __int128 mod)
{
    __int128 result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod128(result, base, mod);
        base = mul_mod128(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Euclidean gcd for 128‑bit */
static __int128 gcd128(__int128 a, __int128 b)
{
    while (b) {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a >= 0 ? a : -a;
}

/* ----------  deterministic Miller–Rabin for 64‑bit and a little beyond  ---------- */

static bool isPrime128(__int128 n)
{
    if (n < 2) return false;
    static const unsigned int small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97
    };
    /* trial division */
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        unsigned int p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* write n-1 = d * 2^s */
    __int128 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const unsigned long long bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        unsigned long long a64 = bases[i];
        if (a64 % (unsigned long long)n == 0) continue;
        __int128 a = a64 % n;
        __int128 x = pow_mod128(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod128(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----------  Pollard‑Rho factorization ---------- */

static __int128 pollard_rho(__int128 n)
{
    if (n % 2 == 0) return 2;
    __int128 x = 2, y = 2, c = 1, d = 1;
    while (d == 1) {
        x = (mul_mod128(x, x, n) + c) % n;
        y = (mul_mod128(y, y, n) + c) % n;
        y = (mul_mod128(y, y, n) + c) % n;
        __int128 diff = x > y ? x - y : y - x;
        d = gcd128(diff, n);
        if (d == n) { /* retry with different constant */
            c++;
            x = 2; y = 2; d = 1;
        }
    }
    return d;
}

/* small primes for trial division (up to 1000) */
static const unsigned int trial_primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,
    167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,
    257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,
    353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,
    449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,
    563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,
    653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,
    761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,
    877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,
    991,997
};
static const size_t trial_prime_cnt = sizeof trial_primes / sizeof trial_primes[0];

/* recursive factorization, filling the array `factors` up to a maximum size */
static void factor128_recursive(__int128 n, __int128 *factors, size_t *fc)
{
    if (n == 1) return;
    if (isPrime128(n)) {
        factors[(*fc)++] = n;
        return;
    }
    __int128 d = pollard_rho(n);
    factor128_recursive(d, factors, fc);
    factor128_recursive(n / d, factors, fc);
}

/* ------------  representation test  ------------ */

static bool can_represent128(__int128 m)
{
    /* factor m and check parity of exponents for primes 5 or 7 mod 8 */
    /* local factors array (enough for numbers < 2^128) */
    __int128 factors[128];
    size_t fc = 0;
    /* trial division by small primes first */
    __int128 tmp = m;
    for (size_t i = 0; i < trial_prime_cnt && (__int128)trial_primes[i] * trial_primes[i] <= tmp; ++i) {
        unsigned int p = trial_primes[i];
        while (tmp % p == 0) {
            factors[fc++] = p;
            tmp /= p;
        }
    }
    if (tmp > 1) {
        if (isPrime128(tmp))
            factors[fc++] = tmp;
        else
            factor128_recursive(tmp, factors, &fc);
    }
    /* parity table */
    struct { __int128 prime; unsigned int parity; } table[128];
    size_t tc = 0;
    for (size_t i = 0; i < fc; ++i) {
        __int128 f = factors[i];
        bool found = false;
        for (size_t j = 0; j < tc; ++j) {
            if (table[j].prime == f) {
                table[j].parity ^= 1;
                found = true;
                break;
            }
        }
        if (!found) {
            table[tc].prime = f;
            table[tc].parity = 1;
            ++tc;
        }
    }
    for (size_t i = 0; i < tc; ++i) {
        unsigned int r = (unsigned int)(table[i].prime & 7); /* lower 3 bits suffice for mod 8 */
        if ((r == 5 || r == 7) && (table[i].parity & 1))
            return false;
    }
    return true;
}

/* --------------  main sequence function -------------- */

seq_t a(idx_t n)
{
    /* n is >=1 */
    __int128 n128 = (__int128)n;
    __int128 n2 = n128 * n128;                       /* n^2 */
    __int128 powk = 3;                                /* 3*2^k, k starts at 0 */
    seq_t cnt = 0;
    while (powk <= n2) {
        __int128 m = n2 - powk;
        if (m == 0 || can_represent128(m))
            cnt++;
        powk <<= 1;                                    /* multiply by 2 for next k */
    }
    return cnt;
}

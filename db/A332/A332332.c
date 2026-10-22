/*
 * A332332  Coefficients of L-series for elliptic curve "33a1": y^2 + x*y =
 *          x^3 + x^2 - 11*x.
 * https://oeis.org/A332332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 2148, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A332332 – Coefficients of L-series for elliptic curve 33a1.
 *
 * The sequence is multiplicative with the following rules:
 *   - a(3^k) = (-1)^k
 *   - a(11^k) = 1
 *   - a(2^k) follows the recurrence a(2^k)=a(2)a(2^{k-1})-2a(2^{k-2}) with a(2)=1
 *   - For other primes p, the coefficient a(p^e) satisfies
 *           a(p^e) = a(p)·a(p^{e-1}) – p·a(p^{e-2})
 *     where a(p) = -(sum_{x=0}^{p-1} ( (x^2 + 4(x^3+x^2-11x)) | p )),
 *     the Legendre symbol.  This formula holds for p≠2,3,11.
 *
 * The function below uses only integer arithmetic and simple
 * multiplicative factorisation to compute a(n) for any n≥1.
 *
 * Author: OpenAI ChatGPT
 * Date: 2026‑10‑19
 */

#include <stdint.h>

/* fast modular exponentiation for 64‑bit integers */
static uint64_t pow_mod_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1U)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        e >>= 1U;
    }
    return res;
}

/* Legendre symbol (x|p) for odd prime p, result in {1,-1,0} */
static int legendre_symbol_uint64(uint64_t x, uint64_t p)
{
    x %= p;
    if (x == 0U) return 0;
    uint64_t t = pow_mod_uint64(x, (p - 1) >> 1, p);
    return (t == p - 1U) ? -1 : 1;
}

/* evaluate a(p) for a prime p ≠ 2,3,11.
 *  a(p) = -( Σ_{x=0}^{p-1} ( (x^2 + 4(x^3+x^2-11x)) | p ) ) */
static seq_t compute_a_prime(uint64_t p)
{
    if (p == 2U) return (seq_t)1;   /* handled separately elsewhere */

    int sum = 0;
    for (uint64_t x = 0; x < p; ++x) {
        /* compute rhs = x^3 + x^2 - 11*x  (mod p) safely */
        uint64_t xsq = (x * x) % p;
        uint64_t xxx = (xsq * x) % p;
        uint64_t term = (xxx + xsq) % p;
        uint64_t sub = (11ULL * x) % p;
        uint64_t rhs = (term + p - sub) % p;    /* rhs ≡ term‑sub (mod p) */

        /* delta = x^2 + 4*rhs (mod p) */
        uint64_t delta = (xsq + (4ULL * rhs) % p) % p;

        sum += legendre_symbol_uint64(delta, p);
    }
    return -(seq_t)sum;
}

/* a(p^e) for any prime p and exponent e≥1 (e is 1‑based) */
static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    if (p == 3U) {
        /* a(3^k) = (-1)^k */
        return (e & 1U) ? (seq_t)-1 : (seq_t)1;
    }
    if (p == 11U) {
        /* a(11^k) = 1 */
        return (seq_t)1;
    }

    seq_t ap = compute_a_prime(p);   /* a(p) */
    if (e == 1U) return ap;

    /* recurrence a(p^k) = a(p)*a(p^{k-1}) – p*a(p^{k-2}) */
    seq_t prev = (seq_t)1;   /* a(p^0) = 1 */
    seq_t curr = ap;        /* a(p^1) = a(p) */
    for (uint64_t k = 2; k <= e; ++k) {
        seq_t next = curr * ap - (seq_t)p * prev;
        prev = curr;
        curr = next;
    }
    return curr;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;  /* undefined below offset, but harmless */

    seq_t result = (seq_t)1;
    idx_t rem = n;

    /* factor 2 */
    if ((rem & 1U) == 0U) {
        uint64_t exp = 0;
        while ((rem & 1U) == 0U) {
            rem >>= 1;
            ++exp;
        }
        result *= a_prime_power(2U, exp);
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0U) {
            uint64_t exp = 0;
            while (rem % p == 0U) {
                rem /= p;
                ++exp;
            }
            result *= a_prime_power((uint64_t)p, exp);
        }
    }

    /* remaining prime > sqrt(rem) */
    if (rem > 1U)
        result *= a_prime_power((uint64_t)rem, 1U);

    return result;
}

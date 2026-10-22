/*
 * A308503  Square array T(n, k), n, k > 0, read by antidiagonals upwards:
 *          T(n, k) = f(g(n) | g(k)), where f is defined over the set of
 *          finite sequences of nonnegative integers with no trailing zero as
 *          f(e) = Sum_{k = 1..#e} prime(k)^e_k, g is the inverse of f, and |
 *          denotes concatenation.
 * https://oeis.org/A308503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(g(i) | g(j)) where (i,j) are determined by reading the
 *            array T(n,k) along antidiagonals upwards (i.e. all pairs with
 *            i+j constant, ordered by decreasing i). f(e) = ∏ prime(k)^e_k
 *            (prime(1)=2, prime(2)=3, …) – product of prime powers. g(n) is
 *            the exponent vector of the prime factorisation...
 * Range:     u8 n <= 48, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes_upto, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/mul_small.h"

/* ---------- prime table, built once on first use --------------------- */
static uint64_t       *prime_list   = NULL;
static size_t          prime_cnt    = 0;
static bool            primes_ready = false;

static void ensure_primes(void)
{
    if (primes_ready) return;
    /* 400 is more than enough for numbers up to about 10 000 */
    size_t limit = 400;
    build_primes_upto((idx_t)limit, (uint64_t **)&prime_list, &prime_cnt);
    primes_ready = true;
}

/* ---------- exponent vector of n ------------------------------------ */
static void exponents_of(idx_t n, size_t *len, unsigned int *exp)
{
    idx_t m = n;
    size_t l = 0;
    for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= m; ++pi)
    {
        unsigned int e = 0;
        while (m % prime_list[pi] == 0) {
            m /= prime_list[pi];
            ++e;
        }
        exp[l++] = e;          /* store zero exponent when e==0 */
    }
    *len = l;                  /* may be 0 when n==1 (empty sequence) */
}

/* ---------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not defined, guard */

    ensure_primes();

    /* ---- find coordinates (i,j) of the nth element (antidiagonals) ---- */
    /* Find minimal s such that s*(s+1)/2 >= n */
    idx_t s = 1;
    idx_t tri = 1;                    /* triangular number s*(s+1)/2 */
    while (tri < n) {
        ++s;
        tri = s * (s + 1) / 2;
    }
    idx_t prev_tri = (s - 1) * s / 2;   /* triangular number of previous diagonal */
    idx_t pos = n - prev_tri - 1;       /* 0‑based position within this diagonal */
    idx_t i = s - pos;                  /* row index (≥1) */
    idx_t j = pos + 1;                  /* column index (≥1) */

    /* ---- exponents of i and j --------------------------------------- */
    unsigned int exp_i[128], exp_j[128];
    size_t len_i, len_j;
    exponents_of(i, &len_i, exp_i);
    exponents_of(j, &len_j, exp_j);

    /* ---- compute T(i,j) = f(g(i)|g(j)) ----------------------------- */
    seq_t res = (seq_t)1;

    /* multiply primes for the first part (exponents of i) */
    for (size_t k = 0; k < len_i; ++k) {
        unsigned int e = exp_i[k];
        for (unsigned int t = 0; t < e; ++t)
            res = mul_small(res, (uint32_t)prime_list[k]); /* prime index k */
    }

    /* multiply primes for the second part (exponents of j),
       using primes starting at index len_i */
    for (size_t k = 0; k < len_j; ++k) {
        unsigned int e = exp_j[k];
        for (unsigned int t = 0; t < e; ++t)
            res = mul_small(res, (uint32_t)prime_list[len_i + k]);
    }

    return res;
}

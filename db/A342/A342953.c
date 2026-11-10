/*
 * A342953  a(n) is the least prime that starts a string of exactly n primes
 *          p_1, p_2, ... p_n where p_{i+1} = A258881(p_i), but A258881(p_n)
 *          is not prime.
 * https://oeis.org/A342953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   search over primes p in increasing order (sieve-assisted); a(n)
 *            = least prime p such that iterating A258881(x) = x + sum of
 *            squared digits of x from p gives exactly n primes before
 *            reaching a non-prime
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

static uint64_t A258881_step(uint64_t x)
{
    uint64_t s = x;
    uint64_t t = x;
    while (t > 0) {
        uint64_t d = t % 10;
        s += d * d;
        t /= 10;
    }
    return s;
}

static unsigned char *A342953_comp = NULL;
static uint64_t A342953_lim = 0;

static void A342953_build_sieve(uint64_t lim)
{
    uint64_t i, j;
    unsigned char *c = (unsigned char *)malloc((size_t)(lim + 1));
    if (!c) return;
    memset(c, 0, (size_t)(lim + 1));
    c[0] = 1;
    if (lim >= 1) c[1] = 1;
    for (i = 2; i * i <= lim; i++) {
        if (!c[i]) {
            for (j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    free(A342953_comp);
    A342953_comp = c;
    A342953_lim = lim;
}

static bool A342953_isp(uint64_t x)
{
    if (x <= A342953_lim) return !A342953_comp[x];
    return is_prime_u64(x);
}

static uint64_t A342953_chain_len(uint64_t p)
{
    uint64_t c = 0;
    while (A342953_isp(p)) {
        c++;
        p = A258881_step(p);
    }
    return c;
}

#define A342953_MAXN 64
static uint64_t A342953_found[A342953_MAXN + 1];
static uint64_t A342953_next = 2;

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    if (target == 0 || target > A342953_MAXN) return (seq_t)0;
    if (A342953_lim == 0) A342953_build_sieve((uint64_t)1 << 20);
    while (A342953_found[target] == 0) {
        uint64_t p = A342953_next;
        uint64_t len;
        if (p > A342953_lim) A342953_build_sieve(A342953_lim * 2);
        if (!A342953_comp[p]) {
            len = A342953_chain_len(p);
            if (len >= 1 && len <= A342953_MAXN && A342953_found[len] == 0)
                A342953_found[len] = p;
        }
        A342953_next = (p == 2) ? 3 : p + 2;
    }
    return (seq_t)A342953_found[target];
}

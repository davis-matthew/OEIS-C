/*
 * A361208  Number of middle divisors of the n-th number whose divisors
 *          increase by a factor of 2 or less.
 * https://oeis.org/A361208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A067742(A174973(n)); search for the n-th number whose
 *            divisors increase by a factor of at most 2, then count its
 *            divisors d with 2*d^2 >= m and d^2 < 2*m
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A361208_terms = NULL;
static size_t A361208_len = 0, A361208_cap = 0;
static uint64_t A361208_next = 1;

static bool A361208_ok(uint64_t m)
{
    uint64_t prod = 1, t = m;
    for (uint64_t p = 2; t > 1; ++p) {
        if (p > 2 * prod) return false;
        if (p * p > t) {
            /* t is prime */
            return t <= 2 * prod;
        }
        if (t % p == 0) {
            do { t /= p; prod *= p; } while (t % p == 0);
        }
    }
    return true;
}

static seq_t A361208_mid(uint64_t m)
{
    uint64_t divs[16384];
    size_t cnt = 1;
    divs[0] = 1;
    uint64_t t = m;
    for (uint64_t p = 2; t > 1; ++p) {
        if (p * p > t) p = t;
        if (t % p == 0) {
            size_t old = cnt;
            uint64_t pk = 1;
            while (t % p == 0) {
                t /= p;
                pk *= p;
                for (size_t i = 0; i < old; ++i) divs[cnt++] = divs[i] * pk;
            }
        }
    }
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned __int128 sq = (unsigned __int128)divs[i] * divs[i];
        if (sq * 2 >= m && sq < (unsigned __int128)m * 2) res++;
    }
    return res;
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    while (A361208_len < target) {
        uint64_t c = A361208_next++;
        if (A361208_ok(c)) {
            if (A361208_len == A361208_cap) {
                A361208_cap = A361208_cap ? A361208_cap * 2 : 1024;
                A361208_terms = (uint64_t *)realloc(A361208_terms, A361208_cap * sizeof(uint64_t));
                if (!A361208_terms) abort();
            }
            A361208_terms[A361208_len++] = c;
        }
    }
    return A361208_mid(A361208_terms[target - 1]);
}

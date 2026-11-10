/*
 * A374129  Numbers k for which gcd(A075255(k), A374127(k)) = abs(A374127(k));
 *          numbers k such that A075255(k) is a multiple of A374127(k).
 * https://oeis.org/A374129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A374127(k) divides A075255(k)
 *            (gcd(A075255(k),A374127(k)) = |A374127(k)|), where A075255(k) =
 *            k - sopfr(k) and A374127(k) = sigma(k) - sopfr(k) - k; found by
 *            search with trial-division factorization
 * Range:     u8 n <= 132, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 7982 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;

static bool qualifies(idx_t k)
{
    idx_t rem = k;
    seq_t sigma = 1;
    seq_t sopfr = 0;
    for (idx_t p = 2; p <= rem / p; ++p) {
        if (rem % p == 0) {
            seq_t term = 1, pw = 1;
            while (rem % p == 0) {
                rem /= p;
                pw *= (seq_t)p;
                term += pw;
                sopfr += (seq_t)p;
            }
            sigma *= term;
        }
    }
    if (rem > 1) {
        sigma *= ((seq_t)rem + 1);
        sopfr += (seq_t)rem;
    }
    seq_t A = (seq_t)k - sopfr;
    seq_t s = sigma - (seq_t)k;          /* sum of proper divisors */
    seq_t mag = (s >= sopfr) ? (s - sopfr) : (sopfr - s);
    if (mag == 0) return A == 0;
    return (A % mag) == 0;
}

seq_t a(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (qualifies(next_k)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}

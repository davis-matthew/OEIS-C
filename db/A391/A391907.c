/*
 * A391907  Denominators of the convergents given by treating A390946 as
 *          continued fraction coefficients after the leading 0.
 * https://oeis.org/A391907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(0) = 1, a(1) = c(1), where c =
 *            A390946 (denominators of the convergents of [0; c(1), c(2),
 *            ...])
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 25, u64 n <= 48, u128 n <= 92,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 1774 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A390946_cache      = NULL;
static size_t A390946_cache_len  = 0;
static size_t A390946_cache_cap  = 0;

static uint32_t *A390946_primes   = NULL;
static size_t A390946_primes_len  = 0;
static size_t A390946_primes_cap  = 0;

static size_t A390946_next_d = 1;
static size_t A390946_next_i = 0;

static void A390946_add_next_prime(void)
{
    uint32_t candidate = (A390946_primes_len == 0) ? 2U : A390946_primes[A390946_primes_len - 1] + 1U;
    for (;; ++candidate) {
        bool prime = true;
        for (size_t p = 0; p < A390946_primes_len; ++p) {
            uint32_t pr = A390946_primes[p];
            uint64_t pr2 = (uint64_t)pr * (uint64_t)pr;
            if (pr2 > candidate) break;
            if (candidate % pr == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (A390946_primes_len == A390946_primes_cap) {
                size_t new_cap = A390946_primes_cap ? A390946_primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t*)realloc(A390946_primes, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                A390946_primes = tmp;
                A390946_primes_cap = new_cap;
            }
            A390946_primes[A390946_primes_len++] = candidate;
            return;
        }
    }
}

static void A390946_ensure_prime_index(size_t need)
{
    while (A390946_primes_len <= need) A390946_add_next_prime();
}

static void A390946_push_term(idx_t q)
{
    if (A390946_cache_len >= A390946_cache_cap) {
        size_t new_cap = A390946_cache_cap ? A390946_cache_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, A390946_cache, new_cap);
        if (!tmp) abort();
        A390946_cache = tmp;
        A390946_cache_cap = new_cap;
    }
    A390946_cache[A390946_cache_len++] = (seq_t)q;
}

static seq_t A390946_a(idx_t n)
{
    if (n == 0) return 0;
    if (A390946_cache_len >= (size_t)n) return A390946_cache[n - 1];

    if (A390946_primes_len == 0) {
        A390946_primes_cap = 16;
        A390946_primes = (uint32_t*)malloc(A390946_primes_cap * sizeof(uint32_t));
        if (!A390946_primes) abort();
        A390946_primes[0] = 2U;
        A390946_primes_len = 1;
    }

    while (A390946_cache_len < (size_t)n) {
        A390946_ensure_prime_index(A390946_next_d);
        uint64_t num = A390946_primes[A390946_next_i];
        uint64_t den = A390946_primes[A390946_next_d];
        while (num > 0) {
            uint64_t q = den / num;
            uint64_t r = den % num;
            A390946_push_term((idx_t)q);
            den = num;
            num = r;
        }
        ++A390946_next_i;
        if (A390946_next_i == A390946_next_d) {
            A390946_next_i = 0;
            ++A390946_next_d;
        }
    }
    return A390946_cache[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t prev = 1;
    seq_t cur = A390946_a(1);
    for (idx_t k = 2; k <= n; ++k) {
        seq_t nxt = A390946_a(k) * cur + prev;
        prev = cur;
        cur = nxt;
    }
    return cur;
}

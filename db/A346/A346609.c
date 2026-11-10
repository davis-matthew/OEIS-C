/*
 * A346609  The successive odd nonprimes produced during the construction of
 *          A203069.
 * https://oeis.org/A346609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = b(n-1) + b(n) where b = A203069
 *            (b(1)=1, b(k) = least unused number of opposite parity to b(k-1)
 *            with b(k-1)+b(k) composite); computed by greedy search
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9479 (2026-11-10)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static bool A203069_isPrimeGeneric(seq_t n)
{
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t *A203069_seq   = NULL;
static size_t A203069_cap   = 0;
static size_t A203069_len   = 0;

static bool *A203069_used   = NULL;
static size_t A203069_ucap  = 0;

static void A203069_ensure_used(size_t idx)
{
    if (idx < A203069_ucap)
        return;
    size_t new_cap = A203069_ucap ? A203069_ucap << 1U : 16;
    while (new_cap <= idx)
        new_cap <<= 1U;
    bool *nu = (bool *)realloc(A203069_used, new_cap * sizeof(bool));
    if (!nu)
        abort();
    memset(nu + A203069_ucap, 0, (new_cap - A203069_ucap) * sizeof(bool));
    A203069_used = nu;
    A203069_ucap = new_cap;
}

static void A203069_ensure_seq(size_t len)
{
    if (len <= A203069_cap)
        return;
    size_t new_cap = A203069_cap ? A203069_cap << 1U : 4;
    while (new_cap < len)
        new_cap <<= 1U;
    seq_t *ns = mem_resize(seq_t, A203069_seq, new_cap);
    if (!ns)
        abort();
    A203069_seq = ns;
    A203069_cap = new_cap;
}

static void A203069_mark(size_t v)
{
    A203069_ensure_used(v);
    A203069_used[v] = true;
}

static void A203069_generate(idx_t n)
{
    while (A203069_len < (size_t)n) {
        if (A203069_len == 0) {
            A203069_ensure_seq(1);
            A203069_seq[0] = 1;
            A203069_mark(1);
            A203069_len = 1;
            continue;
        }
        seq_t prev = A203069_seq[A203069_len - 1];
        bool need_odd = (prev & 1) == 0;
        size_t cand = need_odd ? (size_t)1 : (size_t)2;
        while (true) {
            A203069_ensure_used(cand);
            if (!A203069_used[cand]) {
                seq_t sum = prev + (seq_t)cand;
                if (!A203069_isPrimeGeneric(sum)) {
                    A203069_ensure_seq(A203069_len + 1);
                    A203069_seq[A203069_len++] = (seq_t)cand;
                    A203069_mark(cand);
                    break;
                }
            }
            cand += 2;
        }
    }
}

static seq_t A203069_a(idx_t n)
{
    if (n < 1) return 0;
    A203069_generate(n);
    return A203069_seq[(size_t)(n - 1)];
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 1;
    seq_t x = A203069_a(n);
    seq_t y = A203069_a(n - 1);
    return x + y;
}

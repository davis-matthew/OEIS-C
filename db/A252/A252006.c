/*
 * A252006  a(n) = sum of digits of A252005(n) and A252005(n+1).
 * https://oeis.org/A252006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A007953(A252005(n)) + A007953(A252005(n+1)), where
 *            A252005(1) = 2 and A252005(k) is the least prime > A252005(k-1)
 *            such that digitsum(A252005(k-1)) + digitsum(A252005(k)) is prime
 *            (computed by search)
 * Range:     u8 n <= 10, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-07)
 * Helpers:   digit_sum, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"
#include "../utils/is_prime_seq.h"

static bool A252005_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A252005_is_prime_big(seq_t n)
{
    if (n < 2) return false;
    if (A252005_fits_u64(n)) return is_prime_u64((uint64_t)n);
    return is_prime_seq(n);
}

static seq_t A252005_next_prime_after(seq_t start)
{
    seq_t cand = start + (seq_t)1;
    while (!A252005_is_prime_big(cand))
        ++cand;
    return cand;
}

static seq_t A252005_a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n <= cache_len)
        return cache[n - 1];

    seq_t *new_cache = mem_resize(seq_t, cache, (size_t)n);
    if (!new_cache) abort();
    cache = new_cache;

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;
            continue;
        }
        seq_t prev = cache[i - 2];
        seq_t dprev = digit_sum(prev);
        seq_t cand = A252005_next_prime_after(prev);
        while (true) {
            seq_t sum = dprev + digit_sum(cand);
            if (A252005_is_prime_big(sum))
                break;
            cand = A252005_next_prime_after(cand);
        }
        cache[i - 1] = cand;
    }

    cache_len = n;
    return cache[n - 1];
}

seq_t a(idx_t n)
{
    seq_t x = A252005_a(n);
    seq_t y = A252005_a(n + 1);
    return digit_sum(x) + digit_sum(y);
}

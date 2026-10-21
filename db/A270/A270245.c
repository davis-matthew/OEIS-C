/*
 * A270245  Lesser of twin primes such that sum of twin prime pair is the sum
 *          of 2 nonzero squares.
 * https://oeis.org/A270245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth lesser twin prime p such that 2p + 2 can be
 *            expressed as a^2 + b^2 with a,b > 0. Computed by a simple
 *            brute‑force search of successive twin primes
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

static bool is_sum_of_two_nonzero_squares(seq_t n)
{
    if (n < 2) return false;
    seq_t max_a = floor_sqrt(n / 2);
    for (seq_t a = 1; a <= max_a; ++a)
    {
        seq_t a_sq = a * a;
        seq_t rest = n - a_sq;
        if (rest == 0) continue;
        seq_t b = floor_sqrt(rest);
        if (b > 0 && b * b == rest) return true;
    }
    return false;
}

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;
static uint64_t cand64 = 3;   /* next odd number to test */

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    cache_cap = new_cap;
}

static void append_term(seq_t val)
{
    ensure_cache_cap(cache_size + 1);
    seq_cache[cache_size++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (cache_size < n)
    {
        for (;;)
        {
            if (is_prime((idx_t)cand64) && is_prime((idx_t)(cand64 + 2)))
            {
                uint64_t sum64 = cand64 * 2ULL + 2ULL;
                seq_t sum = (seq_t)sum64;
                if (is_sum_of_two_nonzero_squares(sum))
                {
                    append_term((seq_t)cand64);
                    cand64 += 2;
                    break;
                }
            }
            cand64 += 2;
        }
    }
    return seq_cache[n - 1];
}

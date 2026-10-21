/*
 * A286972  Numbers k such that the average of the prime power divisors (not
 *          including 1) of k is an integer.
 * https://oeis.org/A286972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that the average of its
 *            prime‑power divisors * (excluding 1) is an integer
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic cache for sequence values */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* Next integer to test (starts at 2) */
static uint64_t next_test = 2;

/* Append a value to the cache, expanding storage if necessary */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_mem = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_mem) abort();
        seq_cache = new_mem;
        cache_cap = new_cap;
    }
    seq_cache[cache_len++] = val;
}

/* Test whether k satisfies the condition.
 * cnt  = number of prime‑power divisors (excluding 1)
 * sum  = sum of those prime powers
 * Returns true if sum % cnt == 0. */
static bool qualifies(uint64_t k, uint32_t *cnt, seq_t *sum)
{
    uint64_t tmp = k;
    *cnt = 0;
    *sum = 0;

    /* Handle factor 2 */
    if (tmp % 2 == 0) {
        uint32_t e = 0;
        seq_t sp = 2;
        seq_t s = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            s += sp;
            sp *= 2;
            ++e;
        }
        *cnt += e;
        *sum += s;
    }

    /* Handle odd factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint32_t e = 0;
            seq_t sp = p;
            seq_t s = 0;
            while (tmp % p == 0) {
                tmp /= p;
                s += sp;
                sp *= p;
                ++e;
            }
            *cnt += e;
            *sum += s;
        }
    }

    /* Remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        seq_t s = tmp;  /* exponent 1 */
        *cnt += 1;
        *sum += s;
    }

    return (*cnt > 0) && ((*sum % *cnt) == 0);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset protection */

    /* Generate sequence values until we have n entries */
    while (cache_len < (size_t)n) {
        uint32_t cnt;
        seq_t sum;
        if (qualifies(next_test, &cnt, &sum))
            append_cache(next_test);
        ++next_test;
    }

    return seq_cache[n - 1];
}

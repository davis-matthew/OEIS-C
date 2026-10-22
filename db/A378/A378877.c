/*
 * A378877  Let k = A379336(n). Then a(n) = sum of divisors d | k such that d
 *          neither divides nor is coprime to k/d, and k/d does not divide d.
 * https://oeis.org/A378877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by enumerating integers k that contain at least one
 * unordered divisor pair (d, k/d) for which
 *   - d does not divide k/d and k/d does not divide d
 *   - gcd(d, k/d) > 1
 * a(n) is the sum of all such d for the n‑th such k. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    if (n <= 1) { *cnt = 0; return; }
    if ((n & 1ULL) == 0ULL) {
        pr[c] = 2ULL; ex[c] = 0ULL;
        while ((n & 1ULL) == 0ULL) { ex[c]++; n >>= 1; }
        c++;
    }
    for (uint64_t f = 3ULL; f <= n / f; f += 2ULL) {
        if (n % f == 0ULL) {
            pr[c] = f; ex[c] = 0ULL;
            while (n % f == 0ULL) { ex[c]++; n /= f; }
            c++;
        }
    }
    if (n > 1ULL) { pr[c] = n; ex[c] = 1ULL; c++; }
    *cnt = c;
}

static void generate_divisors(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                              size_t idx, uint64_t cur, uint64_t k, seq_t *sum)
{
    if (idx == cnt) {
        uint64_t d = cur;
        uint64_t q = k / d;
        seq_t g = gcd((seq_t)d, (seq_t)q);
        if (g > (seq_t)1 && (q % d) != 0ULL && (d % q) != 0ULL) {
            *sum += (seq_t)d;
        }
        return;
    }
    uint64_t p   = pr[idx];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        generate_divisors(pr, ex, cnt, idx + 1, val, k, sum);
        val *= p;
    }
}

static seq_t sum_of_qualifying_divisors(uint64_t k)
{
    seq_t sum = 0;
    if (k < 4) return 0;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_uint64(k, pr, ex, &cnt);
    if (cnt == 0) return 0;
    generate_divisors(pr, ex, cnt, 0, 1ULL, k, &sum);
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset is 1 */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t cur  = 2;
    while (len < (size_t)n) {
        seq_t s = sum_of_qualifying_divisors(cur);
        if (s != 0) {
            if (len == cap) {
                size_t new_cap = cap == 0 ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = s;
        }
        ++cur;
    }
    return cache[(size_t)n - 1];
}

/*
 * A289079  Number of orderless same-trees of weight n with all leaves equal
 *          to 1.
 * https://oeis.org/A289079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1; for n > 1, a(n) = Σ_{d|n, d>1}
 *            binomial(a(n/d)+d-1, d)
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 143, u64 n <= 287,
 *            u128 n <= 479, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Cache for memoisation of computed values */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_cap) return;

    size_t new_cap = required * 2;
    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* allocation failure */

    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        free(cache);
    }
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;

    cache    = tmp;
    cache_cap = new_cap;
}

/* Multiplicative binomial coefficient C(n,k) for seq_t arguments.
   k is a small divisor that comfortably fits into idx_t. */
static seq_t binomial_seq(seq_t n, idx_t k)
{
    if (k == 0) return 1;

    seq_t k_seq = (seq_t)k;
    if (k_seq > n) return 0;                 /* safety guard */

    if (k_seq > n - k_seq) k_seq = n - k_seq;

    seq_t res = 1;
    for (idx_t i = 1; i <= (idx_t)k_seq; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t num   = n - k_seq + i_seq;
        seq_t den   = i_seq;

        seq_t g1 = gcd(res, den);
        res /= g1;
        den /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        res *= num;
        res /= den;
    }
    return res;
}

/* The sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside OEIS range */

    ensure_cache(n);

    if (n > 1 && cache[n] != 0) return cache[n];
    if (n == 1) { cache[1] = 1; return 1; }

    seq_t total = 0;
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;      /* complementary divisor */

            /* term for divisor d */
            seq_t val_d = a(e);
            seq_t term_d = binomial_seq(val_d + (seq_t)d - 1, d);
            total += term_d;

            /* term for divisor e, if distinct and > 1 */
            if (e != d && e > 1) {
                seq_t val_e = a(d);
                seq_t term_e = binomial_seq(val_e + (seq_t)e - 1, e);
                total += term_e;
            }
        }
    }

    /* divisor d = n contributes 1 */
    total += 1;

    cache[n] = total;
    return total;
}

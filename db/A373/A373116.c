/*
 * A373116  Fibonacci numbers whose digits product is a positive perfect power
 *          (A001597).
 * https://oeis.org/A373116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   append_candidate, bitlen_seq_t, digit_product, fib, seq_cmp,
 *            seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth Fibonacci number whose digits product is a positive perfect power.
   Duplicate Fibonacci values are omitted.  The function is memoised across calls. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/fib.h"
#include "../utils/digit_product.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/append_candidate.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_cmp.h"

/* Test whether x is a perfect power (a^e with e>=2, 1 considered a perfect power). */
static bool is_perfect_power(seq_t x)
{
    /* 1 (and 0 if ever compared) is a perfect power, product 0 is skipped before */
    static const seq_t one = 1;
    if (seq_cmp(&x, &one) <= 0) return true;  /* x == 1 */

    int max_e = bitlen_seq_t(x);  /* 2^max_e >= x */

    for (int e = 2; e <= max_e; ++e) {
        /* Binary search for integer r such that r^e == x. */
        seq_t low = 2;
        int high_exp = (max_e + e - 1) / e;        /* upper bound for exponent */
        seq_t high = seq_pow((seq_t)2, (idx_t)high_exp);

        while (seq_cmp(&low, &high) <= 0) {
            seq_t mid = (low + high) / 2;
            seq_t p = seq_pow(mid, (idx_t)e);
            int cmp = seq_cmp(&p, &x);
            if (cmp == 0) return true;          /* exact match */
            else if (cmp < 0) low = mid + 1;    /* p < x */
            else high = mid - 1;                /* p > x */
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cached = NULL;    /* stored qualifying Fibonacci numbers */
    static size_t cached_len = 0;   /* number of terms stored */
    static size_t cached_cap = 0;   /* capacity of cached array */
    static idx_t last_k = 0;        /* last Fibonacci index examined */

    /* Return value if already computed */
    if (n <= cached_len && n > 0) return cached[n - 1];

    /* Work until we have at least n terms */
    for (idx_t k = last_k + 1;; ++k) {
        seq_t fk   = fib(k);                 /* F(k) */
        seq_t prod = digit_product(fk);      /* product of its digits */

        static const seq_t zero = 0;
        if (seq_cmp(&prod, &zero) != 0 && is_perfect_power(prod)) {
            /* omit duplicates (only Fibonacci 1 appears twice) */
            if (cached_len == 0 ||
                seq_cmp(&cached[cached_len - 1], &fk) != 0) {
                append_candidate(&cached, &cached_len, &cached_cap, fk);
                if (n <= cached_len) return cached[n - 1];
            }
        }
        last_k = k;
    }
}

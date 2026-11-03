/*
 * A340908  Primitive numbers m without zero digits such that pod(m + pod(m))
 *          = pod(m) where pod is the product of digits, A007954.
 * https://oeis.org/A340908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   digit_product, get_digits, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A340908: Primitive numbers m without zero digits such that
   pod(m + pod(m)) = pod(m) and m does not begin with 1.
   a(1) = 28, a(2) = 214, … */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_product.h"
#include "../utils/get_digits.h"
#include "../utils/num_digits.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, but harmless */

    /* Persistent memoised terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t next_scan = 2;                /* start at 2 */

    /* Ensure capacity for the array */
    if (terms_cap == 0) {
        terms = seq_alloc(16);
        terms_cap = 16;
    } else if (terms_len == terms_cap) {
        terms = seq_realloc(terms, terms_cap, terms_cap * 2);
        terms_cap *= 2;
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        for (;;) {
            seq_t m = (seq_t)next_scan++;
            if (m == 0) continue;              /* safety for overflow */

            /* Skip numbers that begin with 1 */
            int dlen = num_digits(m);
            if (dlen == 0) continue;           /* m is 0 */
            seq_t p10 = pow10(dlen - 1);
            seq_t lead = m / p10;
            if (lead == (seq_t)1) continue;

            /* Compute product of digits and test for zero digits */
            uint8_t buf[64];
            size_t len = 0;
            get_digits(m, buf, &len);

            seq_t prod = 1;                    /* product of digits */
            bool has_zero = false;
            for (size_t i = 0; i < len; ++i) {
                uint8_t d = buf[i];
                if (d == 0) { has_zero = true; break; }
                prod = mul_small(prod, d);
            }
            if (has_zero) continue;

            /* Verify the defining property */
            seq_t plus  = m + prod;
            seq_t pod2 = digit_product(plus);
            if (pod2 == prod) {
                terms[terms_len++] = m;
                break;                      /* next term */
            }
        }
    }

    return terms[n - 1];
}

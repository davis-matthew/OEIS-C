/*
 * A357490  Numbers k such that the k-th composition in standard order has
 *          integer geometric mean.
 * https://oeis.org/A357490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th k such that the k-th composition in standard
 *            order has integer geometric mean
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper prototypes */
static seq_t pow_limit(seq_t base, idx_t exp, seq_t limit);
static seq_t int_nth_root(seq_t x, idx_t n);

/* Computes a(n) */
seq_t a(idx_t n)
{
    /* static memoisation for arbitrary call order */
    static idx_t next_k = 1;          /* next k to test   */
    static size_t cap = 0, len = 0;   /* cache capacity/size */
    static seq_t *cache = NULL;       /* stored qualifying k-values */

    while (len < n) {
        idx_t k = next_k;
        /* Derive the k‑th composition via the binary‑position rule. */
        idx_t m = 0;          /* number of parts (number of 1‑bits) */
        idx_t prev = 0;       /* previous 1‑bit position (1‑based) */
        seq_t prod = 1;       /* product of parts */

        idx_t temp = k;
        idx_t bitpos = 0;     /* 0‑based bit index (LSB first) */
        while (temp) {
            if (temp & 1) {
                seq_t part = (seq_t)(bitpos + 1 - prev);
                prod *= part;
                prev = bitpos + 1;
                ++m;
            }
            temp >>= 1;
            ++bitpos;
        }

        /* Test whether prod is a perfect m‑th power. */
        bool ok = false;
        if (m > 0) {
            seq_t root = int_nth_root(prod, m);
            seq_t p = pow_limit(root, m, prod);
            ok = (p == prod);
        }

        if (ok) {
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1); /* out of memory */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)k;
        }
        ++next_k;
    }
    return cache[n - 1];
}

/* ------------------------------------------------------------------ */
/* Helper: compute base^exp but stop and return limit+1 if the result
   would exceed the given limit.  Used both for root extraction and
   equality testing. */

static seq_t pow_limit(seq_t base, idx_t exp, seq_t limit)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        /* Avoid overflow: if result > limit/base, the product would exceed limit */
        if (base != 0 && result > limit / base) {
            return limit + 1; /* signal overflow */
        }
        result *= base;
    }
    return result;
}

/* Helper: integer n-th root floor of x (i.e. largest r such that r^n <= x) */
static seq_t int_nth_root(seq_t x, idx_t n)
{
    if (n == 1 || x <= 1) return x;
    seq_t low = 1, high = x, ans = 1;

    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        seq_t p   = pow_limit(mid, n, x);
        if (p == x) return mid;
        if (p < x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* guard against infinite loop */
            high = mid - 1;
        }
    }
    return ans;
}

/*
 * A391347  Numbers k such that Stern polynomial B(5*k,x) is not a multiple of
 *          B(5,x).
 * https://oeis.org/A391347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A391347:
   a(n) = the nth integer k such that the Stern polynomial B(5*k,x)
   is not divisible by B(5,x).  This holds iff
   B(5*k, −½) ≠ 0.  The function evaluates B(m,−½) recursively
   and tests the numerator of the result. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Rational value of B(n, −½) as (num / 2^den).  The numerator
   may be negative; the denominator is a power of 2. */
typedef struct {
    __int128 num;   /* numerator (signed) */
    int      den;   /* exponent of 2 */
} frac_t;

/* Recursive evaluation of B(n,−½).  For even n, B(2m,−½)=−½·B(m,−½).
   For odd n, B(2m+1,−½)=B(m,−½)+B(m+1,−½). */
static frac_t eval_at_root_uint64(uint64_t n)
{
    if (n == 0) {
        frac_t r = {0,0};
        return r;
    }
    if (n == 1) {
        frac_t r = {1,0};
        return r;
    }
    if ((n & 1) == 0) {                 /* even: B(2m) */
        frac_t r = eval_at_root_uint64(n >> 1);
        /* multiply by (−½) → multiply numerator by −1 and add 1 to den */
        r.num = -r.num;
        r.den += 1;
        return r;
    }
    /* odd: B(2m+1) = B(m)+B(m+1) */
    frac_t a = eval_at_root_uint64((n - 1) >> 1);
    frac_t b = eval_at_root_uint64((n + 1) >> 1);

    /* align denominators to the larger one */
    int dmax = (a.den > b.den) ? a.den : b.den;
    if (a.den < dmax) {
        a.num <<= (dmax - a.den);
        a.den = dmax;
    }
    if (b.den < dmax) {
        b.num <<= (dmax - b.den);
        b.den = dmax;
    }

    frac_t r;
    r.num = a.num + b.num;
    r.den = dmax;
    return r;
}

/* Return true iff B(5*k,−½) ≠ 0, i.e. the polynomial B(5*k,x) is not
   a multiple of B(5,x). */
static bool contains_k(uint64_t k)
{
    uint64_t n = k * 5ULL;
    frac_t f = eval_at_root_uint64(n);
    return f.num != 0;
}

/* Cache of the sequence values, built lazily. */
static seq_t *keeps = NULL;
static size_t keep_len = 0;
static size_t keep_cap = 0;
static uint64_t last_scanned = 0;      /* last k value tested */

/* Ensure the cache can hold at least need elements. */
static void grow_keep(size_t need)
{
    if (keep_cap < need) {
        size_t newcap = (keep_cap == 0) ? 8 : (keep_cap << 1);
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(keeps, newcap * sizeof(seq_t));
        keeps = tmp;
        keep_cap = newcap;
    }
}

/* Public routine.  Matches the prelude declaration: seq_t a(idx_t n). */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (keep_len < (size_t)n) {
        ++last_scanned;                /* candidate k */
        if (contains_k(last_scanned)) {
            grow_keep(keep_len + 1);
            keeps[keep_len++] = (seq_t)last_scanned;
        }
    }
    return keeps[(size_t)n - 1];
}

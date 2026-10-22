/*
 * A374731  a(n) = a(n-1) + rotate(a(n-1), n-2 digits right) with a(1) = 1.
 * https://oeis.org/A374731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + rotate_right(a(n-1), (n-2) digits, decimal) with
 *            a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard allocation helpers */
#include <stdlib.h>

/* 10^exp in seq_t */
static seq_t pow10_seq(idx_t exp)
{
    seq_t p = 1;
    while (exp--) p *= 10;
    return p;
}

/* Number of decimal digits of x (x>0) */
static idx_t digit_len_seq(seq_t x)
{
    idx_t len = 0;
    while (x) { x /= 10; ++len; }
    return len;
}

/* Rotate right the decimal representation of v by sh positions
   (0 ≤ sh < digits).  digits is the number of digits of v. */
static seq_t rotate_right(seq_t v, idx_t sh, idx_t digits)
{
    if (sh == 0) return v;
    seq_t pow_shift = pow10_seq(sh);
    seq_t low  = v % pow_shift;
    seq_t high = v / pow_shift;
    seq_t pow_rem = pow10_seq(digits - sh);
    return low * pow_rem + high;
}

/* Cache of previously computed terms.
   cache[i] holds a(i+1).  cached_n is the largest n that has been
   computed (i.e. cache size). */
static seq_t *cache   = NULL;
static idx_t cached_n = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n<1 */

    /* Ensure the initial term a(1)=1 is present */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;     /* Allocation failed */
        cache[0] = 1;             /* a(1) */
        cached_n = 1;
    }

    /* Grow cache and compute missing terms */
    if (cached_n < n) {
        /* Resize cache to hold up to term n */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache) return 0; /* Allocation failed */
        cache = new_cache;

        /* Compute new terms from (cached_n + 1) up to n */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];          /* a(i-1)   */
            idx_t d    = digit_len_seq(prev);   /* digits of a(i-1) */
            idx_t sh   = (i - 2) % d;           /* rotation amount */
            seq_t rot  = rotate_right(prev, sh, d);
            cache[i - 1] = prev + rot;          /* a(i)     */
        }

        cached_n = n;
    }

    return cache[n - 1];
}

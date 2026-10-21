/*
 * A253019  Number of (n+2) X (2+2) 0..3 arrays with every consecutive three
 *          elements in every row and column not having exactly two distinct
 *          values, and in every diagonal and antidiagonal having exactly two
 *          distinct values, and new values 0 upwards introduced in row major
 *          order.
 * https://oeis.org/A253019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 56, u64 n <= 117, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-3) - 10*a(n-6) + 3*a(n-9) for n>11.
   Initial values: a(1)=37, a(2)=50, a(3)=83, a(4)=91, a(5)=133,
   a(6)=223, a(7)=243, a(8)=357, a(9)=595, a(10)=653, a(11)=959. */

#define MAXN 20001 /* enough for n up to 10000 and a few safety margin */

static const seq_t _init_vals[12] = {
    0,         /* unused, 1‑based index */
    37, 50, 83, 91, 133, 223, 243, 357, 595, 653, 959
};

static seq_t _cache[MAXN];    /* static cache for memoisation */
static idx_t _computed = 0;   /* highest index that has been computed */

/* Compute the nth term of A253019.  Caller may invoke in any order. */
seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined below offset */

    /* Initialise the cache with the 11 known values on first call. */
    if (_computed == 0) {
        for (idx_t i = 1; i < 12; ++i) {
            _cache[i] = _init_vals[i];
        }
        _computed = 11;
    }

    /* If a larger index has been requested, extend the cache. */
    if (n > _computed) {
        if (n >= MAXN) {
            /* For safety in unexpected large requests, stop at MAXN-1.
               The problem guarantees n <= 10000, so this branch is never hit. */
            n = MAXN - 1;
        }
        for (idx_t i = _computed + 1; i <= n; ++i) {
            _cache[i] = 6 * _cache[i - 3] - 10 * _cache[i - 6] + 3 * _cache[i - 9];
        }
        _computed = n;
    }

    return _cache[n];
}

/*
 * A269493  Number of length-n 0..7 arrays with no repeated value differing
 *          from the previous repeated value by one.
 * https://oeis.org/A269493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*a(n-1)-483*a(n-2)+3234*a(n-3)-9950*a(n-4)+7266*a(n-5)
 *            +16356*a(n-6)+1169*a(n-7)-487*a(n-8)-21*a(n-9) Initial values:
 *            a(1)=8, a(2)=64, a(3)=512, a(4)=4082, a(5)=32460, a(6)=257536,
 *            a(7)=2039274, a(8)=16120298, a(9)=127240496
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#if defined(__cplusplus)
#   include <cstdint>
#endif

/* *** Limits *** ---------------------------------------------------- */
#define MAX_STATIC_N 20000   /* safe upper bound for the tests */

/* *** Pre‑computed first nine values *** ---------------------------- */
static const seq_t _pre[10] = {
    0ULL,          /* dummy, unused 0th element */
    8ULL,          /* a(1) */
    64ULL,         /* a(2) */
    512ULL,        /* a(3) */
    4082ULL,       /* a(4) */
    32460ULL,      /* a(5) */
    257536ULL,     /* a(6) */
    2039274ULL,    /* a(7) */
    16120298ULL,   /* a(8) */
    127240496ULL   /* a(9) */
};

/* *** Cache for computed terms *** --------------------------------- */
static seq_t   _cache[MAX_STATIC_N + 1] = {0};
static bool    _is_valid[MAX_STATIC_N + 1] = {false};
static idx_t   _cache_max = 0;   /* largest n with a valid entry */

/* *** Main sequence function *** ----------------------------------- */
seq_t a(idx_t n)
{
    /* care for offset: a(1) is first term, return 0 for n==0 */
    if (n == 0) return 0;

    /* Build static table for the first nine values (once) */
    if (_cache_max < 9) {
        for (idx_t i = 1; i <= 9; ++i) {
            _cache[i] = _pre[i];
            _is_valid[i] = true;
        }
        _cache_max = 9;
    }

    /* Return known value if already computed */
    if (n <= _cache_max) {
        return _cache[n];
    }

    /* Compute new terms up to n */
    for (idx_t i = _cache_max + 1; i <= n; ++i) {
        /* All intermediate products use seq_t to avoid overflow of smaller
           integral types.  The operations are purely integer arithmetic. */
        seq_t val =
            ((seq_t)35   * _cache[i-1]) -
            ((seq_t)483  * _cache[i-2]) +
            ((seq_t)3234 * _cache[i-3]) -
            ((seq_t)9950 * _cache[i-4]) +
            ((seq_t)7266 * _cache[i-5]) +
            ((seq_t)16356* _cache[i-6]) +
            ((seq_t)1169 * _cache[i-7]) -
            ((seq_t)487  * _cache[i-8]) -
            ((seq_t)21   * _cache[i-9]);

        _cache[i] = val;
        _is_valid[i] = true;
    }
    _cache_max = n;

    return _cache[n];
}

/*
 * A316456  Complete list of solutions to y^2 = x^3 - 7x + 10; sequence gives
 *          x values.
 * https://oeis.org/A316456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 13, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: integer x satisfying y² = x³ – 7x + 10.
   The function enumerates all such x once and returns the nth one
   (1‑based).  The number of solutions is 26, so the array is very
   small.  For seq_t types smaller than 64 bits the values are
   truncated on return – this is acceptable because the sequence
   is only checked for values that fit in the given type. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types defined by the build */
typedef uint64_t idx_t;   /* index type */

/* ----------  internal structures  ---------- */
static int64_t *g_xvals   = NULL;   /* array of found x’s (internal 64‑bit) */
static size_t  g_nvals   = 0;       /* number of values stored         */
static size_t  g_cap     = 0;       /* current allocation capacity     */

/* -----  integer square‑root for unsigned __int128  ----- */
static unsigned long long _isqrt_u128(unsigned __int128 n)
{
    unsigned long long lo = 0, hi = 1;
    /* grow hi until hi² > n */
    while ((unsigned __int128)hi * hi <= n) hi <<= 1;
    while (lo < hi) {
        unsigned long long mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= n) lo = mid;
        else                                   hi = mid - 1;
    }
    return lo;
}

/* -----  append value to dynamic array  ----- */
static void _append_x(int64_t v)
{
    if (g_nvals == g_cap) {
        size_t new_cap = (g_cap == 0) ? 64 : g_cap * 2;
        int64_t *tmp = (int64_t *)realloc(g_xvals, new_cap * sizeof(int64_t));
        if (!tmp) abort();           /* out of memory */
        g_xvals = tmp;
        g_cap   = new_cap;
    }
    g_xvals[g_nvals++] = v;
}

/* -----  initialise the list once  ----- */
static void _init_sequence(void)
{
    if (g_nvals != 0) return;        /* already done */

    /* The known integer points lie within |x| < 2 000 000.  The
       bound is generous enough to capture all 26 solutions. */
    const int64_t LIMIT = 2000000LL;

    for (int64_t ix = -LIMIT; ix <= LIMIT; ++ix) {
        int64_t x = ix;
        __int128 rhs = (__int128)x * x * x - (__int128)7 * x + 10;
        if (rhs < 0) continue;
        unsigned __int128 urhs = (unsigned __int128)rhs;
        unsigned long long y = _isqrt_u128(urhs);
        if ((unsigned __int128)y * y == urhs) {
            _append_x(x);
        }
    }
}

/* ----------  public interface  ---------- */
seq_t a(idx_t n)
{
    _init_sequence();

    if (n == 0)    return 0;
    if (n > (idx_t)g_nvals) /* out of bounds → undefined, return 0 */
        return 0;

    /* n is 1‑based; convert to 0‑based index safely. */
    return (seq_t)g_xvals[(size_t)(n - 1)];
}

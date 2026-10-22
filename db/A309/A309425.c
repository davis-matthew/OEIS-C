/*
 * A309425  Difference x-y of generator pairs (x,y) {x and y coprime and not
 *          both odd, x > y} of primitive Pythagorean triangles, sorted by x
 *          and y (for same x).
 * https://oeis.org/A309425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309425 – difference x‑y of generator pairs (x,y)
   for primitive Pythagorean triangles.
   The pairs are generated in increasing order of x,
   and for equal x in increasing order of y.
   For each pair (x,y) with gcd(x,y)=1, not both odd and x>y,
   a(n) = x – y.
   This implementation keeps a cache of the differences
   and generates new pairs only when a larger index is requested.
*/

#include <stddef.h>
#include <stdlib.h>

/* Euclidean gcd for idx_t -------------------------------------------------- */
static inline idx_t gcd_ui(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache for computed terms (1‑based indexing) -------------------------------- */
static seq_t * _cache = NULL;        /* array of seq_t values               */
static size_t  _cache_cap = 0;       /* current allocated capacity          */
static idx_t   _cache_size = 0;      /* number of terms stored so far       */

/* Next generator pair (x,y) producing a difference, according to the order   */
static idx_t _next_x = 2;             /* current x (>=2)                     */
static idx_t _next_y = 1;             /* current y (>=1)                     */

static seq_t _next_diff(void)
{
    for (;;) {
        /* if we finished all y for this x, move to the next x */
        if (_next_y >= _next_x) {
            ++_next_x;
            _next_y = 1;
        }

        /* check parity (not both odd) and coprimality */
        if (((_next_x & 1) == 1 && (_next_y & 1) == 1) == 0 && /* not both odd */
            gcd_ui(_next_x, _next_y) == 1)                    /* coprime      */
        {
            seq_t d = (seq_t)(_next_x - _next_y);
            ++_next_y;                    /* prepare next y for next call */
            return d;
        }
        ++_next_y;                        /* try next y                */
    }
}

/* Ensure the cache can hold at least `need` terms --------------------------- */
static void _ensure_cache(idx_t need)
{
    if (_cache_size >= need) return;

    /* Grow the cache if necessary */
    if (_cache_cap < (size_t)need) {
        size_t new_cap = (_cache_cap == 0) ? 64 : _cache_cap * 2;
        while (new_cap < (size_t)need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – for safety, abort */
            abort();
        }
        _cache = tmp;
        _cache_cap = new_cap;
    }

    /* Generate missing terms */
    while (_cache_size < need) {
        _cache[_cache_size++] = _next_diff();
    }
}

/* --------------------------------------------------------------------------- */
/* Exported sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(0) is undefined, but we guard against it anyway */
    if (n == 0) return (seq_t)0;
    _ensure_cache(n);
    return _cache[n - 1];
}

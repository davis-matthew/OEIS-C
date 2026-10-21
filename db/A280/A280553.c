/*
 * A280553  Number of n X 7 0..1 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 6*a(n-1) - 12*a(n-2) + 5*a(n-3) +
 *            12*a(n-4) - 12*a(n-5) - 3*a(n-6) + 6*a(n-7) - a(n-9) (n > 12)
 *            Initial values: a(1)=13, a(2)=400, a(3)=1316, a(4)=3182,
 *            a(5)=6386, a(6)=11697, a(7)=20334, a(8)=34360, a(9)=57389,
 *            a(10)=95548, a(11)=159360, a(12)=266756
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 30, u64 n <= 74, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t _init[12] = {
    (seq_t)13, (seq_t)400, (seq_t)1316, (seq_t)3182,
    (seq_t)6386, (seq_t)11697, (seq_t)20334, (seq_t)34360,
    (seq_t)57389, (seq_t)95548, (seq_t)159360, (seq_t)266756
};

static seq_t * _cache = NULL;       /* 1‑based array of computed terms */
static size_t   _cap   = 0;          /* allocated capacity (actual size) */
static idx_t    _size  = 0;          /* highest n whose value is cached */

/* enlarge cache if necessary and compute all missing terms up to n */
static void _ensure(idx_t n)
{
    if (n <= _size) return;

    /* allocate more space if needed */
    size_t new_cap = _cap;
    if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
    if (new_cap > _cap) {
        seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* memory exhausted */
        _cache = tmp;
        _cap   = new_cap;
    }

    /* fill missing entries */
    for (idx_t i = _size + 1; i <= n; ++i) {
        if (i <= 12) {
            _cache[i] = _init[i - 1];
        } else {
            /* use unsigned arithmetic – the recurrence yields a positive result */
            seq_t pos = (seq_t)6 * _cache[i-1]
                      + (seq_t)5 * _cache[i-3]
                      + (seq_t)12 * _cache[i-4]
                      + (seq_t)6 * _cache[i-7];

            seq_t neg = (seq_t)12 * _cache[i-2]
                      + (seq_t)12 * _cache[i-5]
                      + (seq_t)3 * _cache[i-6]
                      + _cache[i-9];

            _cache[i] = pos - neg;
        }
    }
    _size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* below offset – harmless return */
    _ensure(n);
    return _cache[n];
}

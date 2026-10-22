/*
 * A305478  Number of nX4 0..1 arrays with every element unequal to 0, 1, 3, 6
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +5*a(n-3) +a(n-5) -7*a(n-6) -7*a(n-7) -2*a(n-8)
 *            +2*a(n-9) +2*a(n-10) (for n>13) * Initial values: a(1)=5,
 *            a(2)=17, a(3)=21, a(4)=48, a(5)=98, a(6)=192, * a(7)=368,
 *            a(8)=732, a(9)=1480, a(10)=2932, a(11)=5776, * a(12)=11486,
 *            a(13)=22896
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static table of the first 13 terms; index 0 unused */
static const seq_t _precal[14] = {
    (seq_t)0,
    (seq_t)5,   (seq_t)17, (seq_t)21, (seq_t)48, (seq_t)98,
    (seq_t)192, (seq_t)368, (seq_t)732, (seq_t)1480, (seq_t)2932,
    (seq_t)5776, (seq_t)11486, (seq_t)22896
};

/* memoisation cache */
static seq_t * _cache   = NULL;
static idx_t   _cache_cap = 0;
static idx_t   _cache_max = 0;

/* ensure the cache can hold terms up to n (inclusive) */
static void _ensure_cache(idx_t n)
{
    if (_cache == NULL) {
        size_t init_cap = 14;                     /* enough for initial terms */
        _cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        _cache_cap = init_cap;
        _cache[0] = 0;
        for (idx_t i = 1; i < 14; ++i) _cache[i] = _precal[i];
        _cache_max = 13;
    }
    if ((idx_t)(n + 1) > _cache_cap) {
        size_t new_cap = (size_t)(n + 1) * 2;
        _cache = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
        _cache_cap = new_cap;
    }
    for (idx_t i = _cache_max + 1; i <= n; ++i) {
        idx_t i1 = i - 1, i3 = i - 3, i5 = i - 5;
        idx_t i6 = i - 6, i7 = i - 7, i8 = i - 8;
        idx_t i9 = i - 9, i10 = i - 10;

        /* positive part of the recurrence */
        seq_t pos = _cache[i1];
        pos += (seq_t)5 * _cache[i3];
        pos += _cache[i5];
        pos += (seq_t)2 * _cache[i9];
        pos += (seq_t)2 * _cache[i10];

        /* negative part of the recurrence */
        seq_t neg = (seq_t)7 * _cache[i6];
        neg += (seq_t)7 * _cache[i7];
        neg += (seq_t)2 * _cache[i8];

        _cache[i] = pos - neg;                    /* safe: pos >= neg */
    }
    _cache_max = n;
}

/* required public function */
seq_t a(idx_t n)
{
    if (n <= 13) return _precal[n];
    _ensure_cache(n);
    return _cache[n];
}

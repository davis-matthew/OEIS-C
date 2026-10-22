/*
 * A391917  Number of irredundant sets in the n-double cone graph.
 * https://oeis.org/A391917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            3*a(n-1)+a(n-2)-5*a(n-3)+10*a(n-4)-11*a(n-5)-29*a(n-6)+42*a(n-7)-a(n-8)-13*a(n-9)+22*a(n-10)-23*a(n-11)+a(n-12)+4*a(n-13)-7*a(n-14)+17*a(n-15)-10*a(n-16)-3*a(n-17)+5*a(n-18)-4*a(n-19)+a(n-20)+2*a(n-21)-a(n-22)
 *            for n>22; e.g. a(1)=2, a(2)=10, a(3)=17, …, a(22)=19937157602
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _init[22] = {
    2ULL,         10ULL,        17ULL,       122ULL,       257ULL,
    577ULL,      1850ULL,      5626ULL,    16901ULL,     47525ULL,
    140626ULL,  414737ULL,   1223237ULL, 3583450ULL, 10523537ULL,
   30946970ULL,90973445ULL,267322501ULL,785456677ULL,2308418117ULL,
  6784157957ULL,19937157602ULL
};

static const int _coeff[22] = {
     3,  1, -5, 10, -11, -29, 42, -1,
   -13, 22, -23,  1,  4, -7, 17, -10,
   -3,  5, -4,  1,  2, -1
};

static seq_t _cache[10001];
static size_t _max_cached = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n <= 22) return _init[n-1];

    /* initialise first 22 terms once */
    if (_max_cached < 22) {
        for (size_t i = 1; i <= 22; ++i)
            _cache[i] = _init[i-1];
        _max_cached = 22;
    }

    /* extend cache up to n-1 if needed */
    if (_max_cached < (size_t)n) {
        for (size_t i = _max_cached + 1; i < (size_t)n; ++i) {
            seq_t sum = 0;
            for (int k = 0; k < 22; ++k)
                sum += (seq_t)_coeff[k] * _cache[i - k - 1];
            _cache[i] = sum;
        }
        _max_cached = (size_t)n - 1;
    }

    /* compute a(n) itself */
    seq_t sum = 0;
    for (int k = 0; k < 22; ++k)
        sum += (seq_t)_coeff[k] * _cache[(size_t)n - k - 1];
    _cache[(size_t)n] = sum;
    if (_max_cached < (size_t)n) _max_cached = (size_t)n;

    return _cache[(size_t)n];
}

/*
 * A250169  Number of length 4+1 0..n arrays with the sum of adjacent
 *          differences multiplied by some arrangement of +-1 equal to zero.
 * https://oeis.org/A250169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) - 2*a(n-5) + 2*a(n-7)
 *            + a(n-8) + a(n-9) - 2*a(n-10) - a(n-11) + a(n-12) initial
 *            values: a(1)=16, a(2)=119, a(3)=436, a(4)=1269, a(5)=2860,
 *            a(6)=5831, a(7)=10460, a(8)=17765, a(9)=27984, a(10)=42611,
 *            a(11)=61752, a(12)=87477
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 179, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_N 20000  /* enough for n up to about 10 000 */

static const seq_t _base[12] = {
    (seq_t)16,  (seq_t)119,  (seq_t)436,  (seq_t)1269,
    (seq_t)2860, (seq_t)5831, (seq_t)10460, (seq_t)17765,
    (seq_t)27984, (seq_t)42611, (seq_t)61752, (seq_t)87477
};

static seq_t _cache[MAX_N];
static idx_t _max_computed = 0;
static int _initialized = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Ensure the table is initialized */
    if (!_initialized) {
        for (idx_t i = 0; i < 12; ++i)
            _cache[i] = _base[i];
        _max_computed = 12;
        _initialized = 1;
    }

    /* Expand table if needed */
    if (n <= _max_computed)
        return _cache[n-1];

    /* Compute missing terms up to n */
    for (idx_t i = _max_computed + 1; i <= n; ++i) {
        /* Recurrence uses a(i-12) as last term: cache[i-13] */
        seq_t term = _cache[i-2]          /* a(i-1)   */
                   + 2 * _cache[i-3]      /* a(i-2)   */
                   - _cache[i-4]          /* a(i-3)   */
                   - _cache[i-5]          /* a(i-4)   */
                   - 2 * _cache[i-6]      /* a(i-5)   */
                   + 2 * _cache[i-8]      /* a(i-7)   */
                   + _cache[i-9]          /* a(i-8)   */
                   + _cache[i-10]         /* a(i-9)   */
                   - 2 * _cache[i-11]     /* a(i-10)  */
                   - _cache[i-12]         /* a(i-11)  */
                   + _cache[i-13];        /* a(i-12)  */
        _cache[i-1] = term;
    }
    _max_computed = n;
    return _cache[n-1];
}

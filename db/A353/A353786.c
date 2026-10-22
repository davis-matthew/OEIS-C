/*
 * A353786  Number of distinct nonprime numbers of the form 2^k - 1 that
 *          divide n.
 * https://oeis.org/A353786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct nonprime numbers of the form 2^k - 1
 *            that divide n
 * Range:     u8 n <= 9259, u16 n <= 9259, u32 n <= 9259, u64 n <= 9259,
 *            u128 n <= 9259, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static bool _np_m_init = false;
static idx_t _np_m[65];
static size_t _np_m_cnt = 0;

/* Pre‑compute all composite Mersenne numbers 2^k-1 with k ≥ 2 that fit in idx_t. */
static void _init_np_mersenne(void)
{
    for (idx_t k = 2; k <= 63; ++k) {
        idx_t val = ((idx_t)1 << k) - (idx_t)1;
        if (!is_prime(val)) {
            _np_m[_np_m_cnt++] = val;
        }
    }
    /* Handle the special case k = 64: 2^64-1 = ULLONG_MAX, which is composite. */
    idx_t max_val = ~((idx_t)0);
    if (max_val > (idx_t)1 && !is_prime(max_val)) {
        _np_m[_np_m_cnt++] = max_val;
    }
    _np_m_init = true;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        return (seq_t)0;          /* sequence is defined only for n ≥ 1 */
    }
    if (!_np_m_init) {
        _init_np_mersenne();
    }

    /* The number 1 is always a nonprime Mersenne divisor of n. */
    seq_t count = 1;

    for (size_t i = 0; i < _np_m_cnt; ++i) {
        if (n % _np_m[i] == (idx_t)0) {
            ++count;
        }
    }
    return count;
}

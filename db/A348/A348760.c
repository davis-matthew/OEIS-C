/*
 * A348760  For any nonnegative number n with binary expansion Sum_{k >= 0}
 *          b_k * 2^k, a(n) is the real part of f(n) = Sum_{k >= 0}
 *          ((-1)^Sum_{j = 0..k-1} b_j) * (1+i)^k (where i denotes the
 *          imaginary unit); sequence A348761 gives the imaginary part.
 * https://oeis.org/A348760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 8191, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348760 – real part of the Gaussian‐integer mapping
   a(n) = Σ_{k ≥ 0, b_k = 1} (‑1)^popcount(n & ((1ULL<<k)‑1)) * Re((1 + i)^k)   */
#include <stdbool.h>

static seq_t _real_gk[64];
static seq_t _imag_gk[64];
static bool   _gk_inited = false;

static void _init_gk(void)
{
    if (_gk_inited) return;
    _real_gk[0] = 1;
    _imag_gk[0] = 0;
    for (idx_t k = 1; k <= 63; ++k) {
        seq_t a = _real_gk[k-1];
        seq_t b = _imag_gk[k-1];
        _real_gk[k] = a - b;   /* Re((1+i)^k) = Re((1+i) * (a + ib)) */
        _imag_gk[k] = a + b;   /* Im((1+i)^k) = Im((1+i) * (a + ib)) */
    }
    _gk_inited = true;
}

seq_t a(idx_t n)
{
    _init_gk();
    seq_t   sum   = 0;
    unsigned parity = 0;          /* 0 = even, 1 = odd number of set bits seen */
    idx_t   k = 0;
    idx_t   tmp = n;

    while (tmp) {
        if (tmp & (idx_t)1) {
            if (parity)
                sum -= _real_gk[k];
            else
                sum += _real_gk[k];
            parity ^= 1;            /* toggle parity when a new 1-bit is seen */
        }
        tmp >>= 1;
        ++k;
    }

    return sum;
}

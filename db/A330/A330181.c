/*
 * A330181  a(n) = n + floor(ns/r) + floor(nt/r), where r = Pi - 1, s = Pi, t
 *          = Pi + 1.
 * https://oeis.org/A330181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*Pi/(Pi-1)) + floor(n*(Pi+1)/(Pi-1))
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fixed‑point arithmetic with 12 decimal digits of Pi for
   the required precision (Pi * 10^12 = 3141592653589). */

#include <stdbool.h>
#include <stdint.h>

static bool _a_init = false;
static __uint128_t _A_SCALE;   /* 10^12 as 128‑bit integer */
static __uint128_t _A_PI;      /* Pi * 10^12 truncated        */
static __uint128_t _A_R;       /* (Pi-1) * 10^12              */
static __uint128_t _A_S;       /* Pi * 10^12 (same as _A_PI) */
static __uint128_t _A_T;       /* (Pi+1) * 10^12              */

seq_t a(idx_t n)
{
    /* initialize the fixed–point constants on first entry */
    if (!_a_init) {
        _A_SCALE = (__uint128_t)1000000000000ULL;        /* 10^12   */
        _A_PI    = (__uint128_t)3141592653589ULL;        /* floor(Pi*10^12) */
        _A_R     = _A_PI - _A_SCALE;                    /* (Pi-1)*10^12 */
        _A_S     = _A_PI;                               /* Pi*10^12      */
        _A_T     = _A_PI + _A_SCALE;                    /* (Pi+1)*10^12  */
        _a_init = true;
    }

    /* compute floor(n*s/r) and floor(n*t/r) using 128‑bit arithmetic */
    __uint128_t ns_mul = (__uint128_t)n * _A_S;
    __uint128_t nt_mul = (__uint128_t)n * _A_T;

    seq_t ns_div = (seq_t)(ns_mul / _A_R);
    seq_t nt_div = (seq_t)(nt_mul / _A_R);

    /* final result: a(n) = n + floor(n*s/r) + floor(n*t/r) */
    return (seq_t)n + ns_div + nt_div;
}

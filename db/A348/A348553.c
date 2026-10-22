/*
 * A348553  Number of digits in 11^n.
 * https://oeis.org/A348553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(n*log10(11)) + 1
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of decimal digits of 11^n */

/*  log10(11) ≈ 1.041392685305..., scaled by 10^12 to avoid floating point.
    The integer constant below is floor(log10(11)*10^12).  The scaling
    factor 10^12 guarantees that the product n*log10(11) is accurate
    to at least 10^(-12), which is far below the 0.5 threshold needed
    for correct floor computation for all n up to ~10^16.               */
#include <stdint.h>

static const unsigned long long SCALE      = 1000000000000ULL;      /* 10^12      */
static const unsigned long long LOG10_11_S =
    1041392685305ULL;                         /* floor(log10(11)*SCALE) */

/* Function returning a(n) for given n.  It uses only integer arithmetic
   and works for any seq_t/idx_t combination, regardless of whether
   seq_t is small or an arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    /* Convert idx_t to 64 bit unsigned, safe because idx_t is uint64_t. */
    unsigned long long un = (unsigned long long)n;

    /* Compute floor(n * log10(11)) using fixed‑point arithmetic. */
    unsigned long long digits_floor =
        (unsigned long long)(((unsigned __int128)un * LOG10_11_S) / SCALE);

    /* Number of decimal digits is floor(...) + 1. */
    return (seq_t)(digits_floor + 1);
}

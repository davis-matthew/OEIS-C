/*
 * A318435  Decimal expansion of the nominal Jovian mass parameter in m^3
 *          s^-2.
 * https://oeis.org/A318435
 *
 * Offset:    18    (a(18) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = the (n-17)-th decimal digit of 1.2668653e17
 *            (126686530000000000). a(18) is the first term
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The integer value of 1.2668653 × 10^17.  It fits in 64 bits. */
static const uint64_t JOVIAN_PARAM = 126686530000000000ULL;

/* Precomputed powers of 10 up to 10^17.  Index i holds 10^i. */
static const uint64_t POW10[18] = {
    1ULL,                      /* 10^0 */
    10ULL,                     /* 10^1 */
    100ULL,                    /* 10^2 */
    1000ULL,                   /* 10^3 */
    10000ULL,                  /* 10^4 */
    100000ULL,                 /* 10^5 */
    1000000ULL,                /* 10^6 */
    10000000ULL,               /* 10^7 */
    100000000ULL,              /* 10^8 */
    1000000000ULL,             /* 10^9 */
    10000000000ULL,            /* 10^10 */
    100000000000ULL,           /* 10^11 */
    1000000000000ULL,          /* 10^12 */
    10000000000000ULL,         /* 10^13 */
    100000000000000ULL,        /* 10^14 */
    1000000000000000ULL,       /* 10^15 */
    10000000000000000ULL,      /* 10^16 */
    100000000000000000ULL      /* 10^17 */
};

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 18.  Return 0 for n < 18 or
       beyond the 18 digits. */
    if (n < 18)
        return 0;

    idx_t pos = n - 18;          /* 0‑based index into the 18 digits */
    if (pos >= 18)
        return 0;

    /* The (pos)-th digit is: floor( JOVIAN_PARAM / 10^(17-pos) ) % 10 */
    uint64_t power   = POW10[17 - pos];
    uint64_t divisor = JOVIAN_PARAM / power;
    uint64_t digit   = divisor % 10ULL;

    return (seq_t)digit;
}

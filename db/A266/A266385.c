/*
 * A266385  a(n) = floor(10^k/n) where k is the smallest integer such that the
 *          whole first period or the whole terminating fractional part of the
 *          decimal expansion of 1/n is shifted to appear before the decimal
 *          point in 10^k/n.
 * https://oeis.org/A266385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(10^( max(v2(n),v5(n)) + ord_m(10) ) / n) where m =
 *            n / (2^v2(n) * 5^v5(n)) and ord_m(10) is the multiplicative
 *            order of 10 modulo m (0 if m = 1)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   multiplicative_order_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/multiplicative_order_uint64.h"
/* Ensure 128‑bit support if available */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 uint128_t;
#else
typedef unsigned long long uint128_t;
#endif

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Factor out powers of 2 and 5 */
    uint64_t temp = (uint64_t)n;
    idx_t a2 = 0, b5 = 0;
    while ((temp & 1ULL) == 0) { temp >>= 1; ++a2; }
    while (temp % 5ULL == 0) { temp /= 5; ++b5; }
    uint64_t m = temp;                  /* coprime to 10 */
    idx_t rmax = a2 > b5 ? a2 : b5;     /* length of non‑repeating part */

    /* Compute period length */
    idx_t p = 0;
    if (m != 1) p = multiplicative_order_uint64(m);

    /* Total exponent of 10 in the needed shift */
    idx_t e = rmax + p;

    /* Long division of 10^e by n, digit by digit */
    seq_t quotient = 0;
    uint64_t rem = 1;                     /* leading 1 of 10^e */
    uint64_t digit = rem / n;
    rem %= n;
    quotient = (seq_t)((uint128_t)quotient * 10 + digit);

    for (idx_t i = 1; i <= e; ++i) {
        rem = rem * 10;                   /* next digit is 0 */
        digit = rem / n;
        rem %= n;
        quotient = (seq_t)((uint128_t)quotient * 10 + digit);
    }
    return quotient;
}

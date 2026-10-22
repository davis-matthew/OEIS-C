/*
 * A361010  Decimal expansion of conventional value of ohm-90 (Omega_{90}).
 * https://oeis.org/A361010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn, easy
 * Formula:   a(n) = digit_(n-1) of the decimal expansion of 1 +
 *            98251739507564459 / 5521725026748260492435541 (a(1) = integer
 *            part 1, a(2) = first decimal digit, …)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   _str_to_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_str_to_uint128.h"

static bool _initialized = false;
static unsigned __int128 _N = 0;
static unsigned __int128 _D = 0;

seq_t a(idx_t n) {
    /* One‑time initialization of the rational constants */
    if (!_initialized) {
        _N = _str_to_uint128("98251739507564459");
        _D = _str_to_uint128("5521725026748260492435541");
        _initialized = true;
    }

    if (n == 1)
        return (seq_t)1; /* integer part */

    /* Compute the (n‑1)th decimal digit of (N/D) */
    unsigned __int128 rem = _N;
    idx_t i;
    for (i = 2; i <= n; ++i) {
        rem *= 10;                     /* shift one decimal place */
        unsigned int digit = (unsigned int)(rem / _D);  /* 0 … 9 */
        rem %= _D;
        if (i == n)
            return (seq_t)digit;
    }
    return (seq_t)0; /* unreachable */
}

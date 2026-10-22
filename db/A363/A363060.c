/*
 * A363060  Numbers k such that 5 is the first digit of 2^k.
 * https://oeis.org/A363060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th exponent k for which the first digit of 2^k is
 *            5. The algorithm uses scaled fractional logarithms: for each k
 *            we compute f_k = k * log10(2) (mod 1). If 10^f_k lies in [5,6)
 *            the first digit is 5. We avoid floating‑point by using a
 *            20‑digit fixed‑point representation
 * Range:     u8 n <= 22, u16 n <= 5189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   parse_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/parse_uint128.h"

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* lazy initialization of constants */
    static unsigned __int128 SCALE   = 0;
    static unsigned __int128 LOG2S   = 0;
    static unsigned __int128 THRESH5 = 0;
    static unsigned __int128 THRESH6 = 0;
    static int initialized = 0;

    if (!initialized) {
        /* SCALE = 10^20 */
        SCALE = parse_uint128("10000000000000000000");
        SCALE *= 10;                      /* 10^20 */

        LOG2S   = parse_uint128("30102999566398119521"); /* floor(log10(2)*1e20) */
        THRESH5 = parse_uint128("69897000433601880479"); /* ceil(log10(5)*1e20)  */
        THRESH6 = parse_uint128("77815125038364363251"); /* floor(log10(6)*1e20) */
        initialized = 1;
    }

    unsigned __int128 sum = 0;  /* scaled fractional part: k*log10(2) mod 1 */
    idx_t found = 0;
    idx_t k = 0;

    while (found < n) {
        ++k;
        sum += LOG2S;
        if (sum >= SCALE) sum -= SCALE;

        if (sum >= THRESH5 && sum < THRESH6)
            ++found;
    }

    return (seq_t)k;
}

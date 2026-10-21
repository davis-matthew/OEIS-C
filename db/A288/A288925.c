/*
 * A288925  a(n) = a(n-1) + a(n-2) + 3*a(n-4) - 2*a(n-5) for n >= 5, where
 *          a(0) = 2, a(1) = 4, a(2) = 8, a(3) = 13, a(4) = 26.
 * https://oeis.org/A288925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-4) - 2*a(n-5) Initial values:
 *            a(0)=2, a(1)=4, a(2)=8, a(3)=13, a(4)=26 (OEIS sequence A288925)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *buf   = NULL;    /* array of computed values   */
    static size_t cap   = 0;       /* allocated capacity         */
    static size_t len   = 0;       /* number of values present   */

    /* Ensure capacity at least n+1 */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap << 1 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        buf = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise initial 5 values (only on first use) */
    if (len == 0) {
        buf[0] = (seq_t)2;
        buf[1] = (seq_t)4;
        buf[2] = (seq_t)8;
        buf[3] = (seq_t)13;
        buf[4] = (seq_t)26;
        len = 5;
    }

    /* Return immediately if already computed */
    if ((size_t)n < len) return buf[n];

    /* Extend computation up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
#if defined(__cplusplus)
        /* C++ path: use 128‑bit intermediate only for integral types */
        if constexpr (std::is_integral<seq_t>::value && std::is_unsigned<seq_t>::value) {
            unsigned __int128 val =
                (unsigned __int128)buf[i-1] +
                (unsigned __int128)buf[i-2] +
                3u * (unsigned __int128)buf[i-4] -
                2u * (unsigned __int128)buf[i-5];
            buf[i] = (seq_t)val;
        } else {
            /* Non‑integral (big‑int) types – rely on built‑in operators */
            buf[i] = buf[i-1] + buf[i-2] + buf[i-4] * (seq_t)3 - buf[i-5] * (seq_t)2;
        }
#else
        /* Plain C: assume seq_t is an unsigned integer (uint8_t…uint64_t) */
        unsigned __int128 val =
            (unsigned __int128)buf[i-1] +
            (unsigned __int128)buf[i-2] +
            3u * (unsigned __int128)buf[i-4] -
            2u * (unsigned __int128)buf[i-5];
        buf[i] = (seq_t)val;
#endif
    }

    len = (size_t)n + 1;
    return buf[n];
}

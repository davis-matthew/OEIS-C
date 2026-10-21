/*
 * A274747  Number of nX6 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (0,-1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) -10*a(n-2) -80*a(n-3) +177*a(n-4) -152*a(n-5)
 *            +56*a(n-6) -a(n-7) -5*a(n-8) +a(n-9) Initial values: a(1) = 16,
 *            a(2) = 164, a(3) = 2017, a(4) = 25585, a(5) = 327123, a(6) =
 *            4190533, a(7) = 53680592, a(8) = 687685512, a(9) = 8809672678,
 *            a(10) = 112857546696, a(11) = 1445776873145
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* initial 11 terms of the sequence */
    static const seq_t init[] = {
        16ULL,                /* a(1) */
        164ULL,               /* a(2) */
        2017ULL,             /* a(3) */
        25585ULL,            /* a(4) */
        327123ULL,           /* a(5) */
        4190533ULL,          /* a(6) */
        53680592ULL,         /* a(7) */
        687685512ULL,        /* a(8) */
        8809672678ULL,       /* a(9) */
        112857546696ULL,     /* a(10) */
        1445776873145ULL     /* a(11) */
    };

    /* for n <= 11 simply return the table value */
    if (n <= 11)
        return init[n-1];

    /* buffer of the last 9 values: buf[0] = a(i-1), buf[8] = a(i-9) */
    seq_t buf[9];
    for (int j = 0; j < 9; ++j)
        buf[j] = init[10 - j];   /* init[10] = a(11) ... init[2] = a(3) */

#ifdef __cplusplus
    /* C++ path – use type traits to decide whether to use __int128
       (for builtin integer types) or direct operations (for big integer
       classes). */
    if constexpr (std::is_integral<seq_t>::value) {
        for (idx_t i = 12; i <= n; ++i) {
            __int128 cur = 0;
            cur += (__int128)buf[0] * 14;
            cur -= (__int128)buf[1] * 10;
            cur -= (__int128)buf[2] * 80;
            cur += (__int128)buf[3] * 177;
            cur -= (__int128)buf[4] * 152;
            cur += (__int128)buf[5] * 56;
            cur -= (__int128)buf[6];
            cur -= (__int128)buf[7] * 5;
            cur += (__int128)buf[8];
            seq_t next = (seq_t)cur;
            for (int k = 8; k > 0; --k) buf[k] = buf[k-1];
            buf[0] = next;
        }
        return buf[0];
    } else {
        for (idx_t i = 12; i <= n; ++i) {
            seq_t next = buf[0] * 14
                       - buf[1] * 10
                       - buf[2] * 80
                       + buf[3] * 177
                       - buf[4] * 152
                       + buf[5] * 56
                       - buf[6]
                       - buf[7] * 5
                       + buf[8];
            for (int k = 8; k > 0; --k) buf[k] = buf[k-1];
            buf[0] = next;
        }
        return buf[0];
    }
#else
    /* C path – use signed __int128 for the intermediate calculation. */
    for (idx_t i = 12; i <= n; ++i) {
        __int128 cur = 0;
        cur += (__int128)buf[0] * 14;
        cur -= (__int128)buf[1] * 10;
        cur -= (__int128)buf[2] * 80;
        cur += (__int128)buf[3] * 177;
        cur -= (__int128)buf[4] * 152;
        cur += (__int128)buf[5] * 56;
        cur -= (__int128)buf[6];
        cur -= (__int128)buf[7] * 5;
        cur += (__int128)buf[8];
        seq_t next = (seq_t)cur;
        for (int k = 8; k > 0; --k) buf[k] = buf[k-1];
        buf[0] = next;
    }
    return buf[0];
#endif
}

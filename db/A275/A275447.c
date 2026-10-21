/*
 * A275447  Sum of the asymmetry degrees of all compositions of n with parts
 *          in {2,1,3,5,7,9,...}.
 * https://oeis.org/A275447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) * +
 *            2*a(n-1)+2*a(n-2)-2*a(n-3)-3*a(n-4)-2*a(n-5)-2*a(n-6) * +
 *            2*a(n-7)+3*a(n-8)-a(n-10) * where p(3)=2, p(5)=-2 and p(n)=0
 *            otherwise. * Initial values: a(0)=a(1)=a(2)=0. * This recurrence
 *            follows from the rational generating * function
 *            g(z)=2*z^3*(1-z^2)/((1+z^2)*(1-z-2*z^2+z^4)^2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache can hold indices 0..n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
#ifdef __cplusplus
        cache = static_cast<seq_t*>(realloc(cache, new_size * sizeof(seq_t)));
#else
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
        /* initialise new entries to 0 */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* constants for the recurrence */
    const int coeff[10] = { /* D1..D10 with sign changed for RHS */
        2,  /* a(n-1)   coefficient  +2 */
        2,  /* a(n-2)   coefficient  +2 */
        -2, /* a(n-3)   coefficient  -2 */
        -3, /* a(n-4)   coefficient  -3 */
        -2, /* a(n-5)   coefficient  -2 */
        -2, /* a(n-6)   coefficient  -2 */
        2,  /* a(n-7)   coefficient  +2 */
        3,  /* a(n-8)   coefficient  +3 */
        0,  /* a(n-9)   skip */
        -1  /* a(n-10)  coefficient  -1 */
    };

    /* compute missing values up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < cache_size && cache[i] != 0) /* already computed */
            continue;

#ifdef __cplusplus
        /* If seq_t is unsigned, use 128‑bit signed intermediate
         * to avoid overflow and unsigned wrap‑around. */
        if constexpr (std::is_unsigned<seq_t>::value) {
#endif
            __int128 val = 0;
            if (i == 3) val += 2;
            if (i == 5) val -= 2;
            if (i >= 1) val += (__int128)coeff[0] * (__int128)cache[i-1];
            if (i >= 2) val += (__int128)coeff[1] * (__int128)cache[i-2];
            if (i >= 3) val += (__int128)coeff[2] * (__int128)cache[i-3];
            if (i >= 4) val += (__int128)coeff[3] * (__int128)cache[i-4];
            if (i >= 5) val += (__int128)coeff[4] * (__int128)cache[i-5];
            if (i >= 6) val += (__int128)coeff[5] * (__int128)cache[i-6];
            if (i >= 7) val += (__int128)coeff[6] * (__int128)cache[i-7];
            if (i >= 8) val += (__int128)coeff[7] * (__int128)cache[i-8];
            if (i >= 10) val += (__int128)coeff[9] * (__int128)cache[i-10];
            cache[i] = (seq_t)val;
#ifdef __cplusplus
        } else {
            /* seq_t is a signed or arbitrary‑precision type; use native arithmetic. */
            seq_t val = 0;
            if (i == 3) val += (seq_t)2;
            if (i == 5) val -= (seq_t)2;
            for (int k = 1; k <= 10; ++k) {
                if (i >= (idx_t)k)
                    val += (seq_t)coeff[k-1] * cache[i-k];
            }
            cache[i] = val;
        }
#endif
    }

    return cache[n];
}

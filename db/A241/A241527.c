/*
 * A241527  a(n) = n^3 + (3^n+1)/2.
 * https://oeis.org/A241527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + (3^n + 1)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Compute n^3 */
    seq_t nt   = (seq_t)n;
    seq_t cube = nt * nt * nt;

    /* Compute 3^n */
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 p = 1;
        for (idx_t i = 0; i < n; ++i) p *= (unsigned __int128)3;
        seq_t half = (seq_t)((p + (unsigned __int128)1) / (unsigned __int128)2);
        return cube + half;
    } else {
        seq_t power = (seq_t)1;
        for (idx_t i = 0; i < n; ++i) power *= (seq_t)3;
        seq_t half = (power + (seq_t)1) / (seq_t)2;
        return cube + half;
    }
#else
    unsigned __int128 p = 1;
    for (idx_t i = 0; i < n; ++i) p *= (unsigned __int128)3;
    seq_t half = (seq_t)((p + (unsigned __int128)1) / (unsigned __int128)2);
    return cube + half;
#endif
}

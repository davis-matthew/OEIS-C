/*
 * A249753  Number of triangles in the complex obtained by starting with an
 *          isosceles right triangle, and dividing each cell into two similar
 *          isosceles right triangles n times.
 * https://oeis.org/A249753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 3; a(2) = 7; a(2k+1) =
 *            (140*2^(3k)+318*2^(2k)+60*2^k-48+16*(-1)^k) / 144 (k>0) a(2k) =
 *            (50*2^(3k)+147*2^(2k)+60*2^k-48+16*(-1)^k) / 144 (k>1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;

    if (n & 1) {                    /* n = 2k+1,  k >= 1 */
        idx_t k = (n - 1) >> 1;
#if defined(__cplusplus)
        if constexpr (std::is_integral<seq_t>::value) {
            __int128 num = 0;
            num += (__int128)140 * ((__int128)pow2(3 * k));
            num += (__int128)318 * ((__int128)pow2(2 * k));
            num += (__int128)60  * ((__int128)pow2(k));
            num += (__int128)(-48);
            num += (__int128)(16 * ((k & 1) ? -1 : 1));
            return (seq_t)(num / 144);
        } else {
            seq_t num = (seq_t)140 * pow2(3 * k);
            num += (seq_t)318 * pow2(2 * k);
            num += (seq_t)60 * pow2(k);
            num -= (seq_t)48;
            num += (seq_t)(16 * ((k & 1) ? -1 : 1));
            return num / 144;
        }
#else
        __int128 num = 0;
        num += (__int128)140 * ((__int128)pow2(3 * k));
        num += (__int128)318 * ((__int128)pow2(2 * k));
        num += (__int128)60  * ((__int128)pow2(k));
        num += (__int128)(-48);
        num += (__int128)(16 * ((k & 1) ? -1 : 1));
        return (seq_t)(num / 144);
#endif
    } else {                        /* n = 2k,  k >= 2  */
        idx_t k = n >> 1;           /* k >= 2 */
#if defined(__cplusplus)
        if constexpr (std::is_integral<seq_t>::value) {
            __int128 num = 0;
            num += (__int128)50  * ((__int128)pow2(3 * k));
            num += (__int128)147 * ((__int128)pow2(2 * k));
            num += (__int128)60  * ((__int128)pow2(k));
            num += (__int128)(-48);
            num += (__int128)(16 * ((k & 1) ? -1 : 1));
            return (seq_t)(num / 144);
        } else {
            seq_t num = (seq_t)50 * pow2(3 * k);
            num += (seq_t)147 * pow2(2 * k);
            num += (seq_t)60 * pow2(k);
            num -= (seq_t)48;
            num += (seq_t)(16 * ((k & 1) ? -1 : 1));
            return num / 144;
        }
#else
        __int128 num = 0;
        num += (__int128)50  * ((__int128)pow2(3 * k));
        num += (__int128)147 * ((__int128)pow2(2 * k));
        num += (__int128)60  * ((__int128)pow2(k));
        num += (__int128)(-48);
        num += (__int128)(16 * ((k & 1) ? -1 : 1));
        return (seq_t)(num / 144);
#endif
    }
}

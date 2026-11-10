/*
 * A351944  a(n) = A003557(A181819(n)).
 * https://oeis.org/A351944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003557(A181819(n)); if n = Prod p_i^e_i, then a(n) =
 *            Prod over e of prime(e)^(c_e - 1), where c_e = number of i with
 *            e_i = e and c_e >= 1
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9914 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const unsigned pr[64] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
        179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
        233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
        283, 293, 307, 311
    };
    unsigned cnt[65];
    for (int i = 0; i < 65; i++) cnt[i] = 0;

    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) { t /= p; e++; }
            cnt[e]++;
        }
    }
    if (t > 1) cnt[1]++;

    seq_t result = 1;
    for (int e = 1; e <= 64; e++) {
        for (unsigned j = 1; j < cnt[e]; j++)
            result *= (seq_t)pr[e - 1];
    }
    return result;
}

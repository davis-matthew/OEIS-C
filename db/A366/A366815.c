/*
 * A366815  Hyper-Wiener index in diamond nanowires obtained by connecting n
 *          unit cells in a sequence.
 * https://oeis.org/A366815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (338*n^4 + 481*n^3 + 145*n^2 + 416*n - 36)/3
 * Range:     u8 none, u16 n <= 4, u32 n <= 78, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;
    unsigned long long term =
        (338ULL*nn*nn*nn*nn
         + 481ULL*nn*nn*nn
         + 145ULL*nn*nn
         + 416ULL*nn
         - 36ULL) / 3ULL;
    return (seq_t)term;
}

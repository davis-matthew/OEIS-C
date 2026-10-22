/*
 * A363734  a(n) = Sum_{k=0..n} n^divides(k, n), where divides(k, n) = 1 if k
 *          divides n, otherwise 0.
 * https://oeis.org/A363734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - 1)*tau(n) + n + 1 for n>=1, a(0) = 0
 * Range:     u8 n <= 29, u16 n <= 1679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }

    uint64_t nn = n;
    uint64_t tau = 1;
    uint64_t temp = nn;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                cnt++;
            }
            tau *= (cnt + 1);
        }
    }

    if (temp > 1) {
        tau *= 2;
    }

    seq_t nn_seq = (seq_t)nn;
    seq_t result = (nn_seq - 1) * ((seq_t)tau);
    result += nn_seq;
    result += 1;
    return result;
}

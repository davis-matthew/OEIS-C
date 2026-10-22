/*
 * A380883  a(n) is the smallest multiple of prime(n) which contains every
 *          decimal digit of prime(n), including repetitions.
 * https://oeis.org/A380883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 847, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = minimal m*prime(n) with m∈{3,…,10} such that every digit of prime(n)
 * (including repetitions) appears in m*prime(n). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n) {
    uint32_t p = get_prime(n);               /* prime(n) */
    unsigned char primeCnt[10] = {0};
    digit_counts((seq_t)p, primeCnt);

    for (int m = 3; m <= 10; ++m) {
        seq_t cand = (seq_t)m * (seq_t)p;
        unsigned char candCnt[10] = {0};
        digit_counts(cand, candCnt);
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (candCnt[d] < primeCnt[d]) {
                ok = false;
                break;
            }
        }
        if (ok) return cand;
    }

    /* Fallback: by definition a solution always exists with m <= 10. */
    return (seq_t)10 * (seq_t)p;
}

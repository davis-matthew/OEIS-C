/*
 * A317756  Number of distinct primes obtained by cyclically shifting the
 *          decimal digits of the n-th prime.
 * https://oeis.org/A317756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of distinct primes among cyclic rotations of the
 *            n-th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) /* undefined for n = 0, return 0 */
        return (seq_t)0;

    /* Get the n-th prime (1‑based index). */
    uint32_t p = get_prime(n);

    /* Convert prime to string to obtain its decimal digits. */
    char buf[32];
    int len = snprintf(buf, sizeof buf, "%u", p);
    if (len <= 0)
        return (seq_t)0; /* safety check */

    /* Count distinct prime rotations. */
    seq_t count = 0;
    idx_t seen[20];
    size_t seen_cnt = 0;

    for (int shift = 0; shift < len; ++shift) {
        idx_t val = 0;
        for (int i = 0; i < len; ++i) {
            int d = buf[(shift + i) % len] - '0';
            val = val * 10 + d;
        }

        if (is_prime(val)) {
            bool present = false;
            for (size_t j = 0; j < seen_cnt; ++j) {
                if (seen[j] == val) {
                    present = true;
                    break;
                }
            }
            if (!present) {
                seen[seen_cnt++] = val;
                ++count;
            }
        }
    }
    return count;
}

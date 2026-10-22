/*
 * A345985  Hamming distance between prime(n) and prime(n+1) in base 10.
 * https://oeis.org/A345985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Hamming distance in base 10 between prime(n) and
 *            prime(n+1)
 * Range:     u8 n <= 53, u16 n <= 7181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided get_prime and seq_to_str utilities. */

#include "../utils/get_prime.h"
#include "../utils/seq_to_str.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    /* Retrieve the nth prime and the next prime. */
    seq_t p1 = get_prime(n);
    seq_t p2 = get_prime(n + 1);

    /* Convert each prime to a decimal string. */
    char buf1[32] = {0};
    char buf2[32] = {0};
    size_t len1 = 0, len2 = 0;
    seq_to_str(p1, buf1, &len1);
    seq_to_str(p2, buf2, &len2);

    /* Align right‑justified and compare digits, padding missing digits with 0. */
    size_t maxlen = (len1 > len2) ? len1 : len2;
    seq_t dist = 0;
    for (size_t i = 0; i < maxlen; ++i) {
        int d1 = (i < len1) ? (buf1[len1 - 1 - i] - '0') : 0;
        int d2 = (i < len2) ? (buf2[len2 - 1 - i] - '0') : 0;
        if (d1 != d2) {
            dist++;
        }
    }
    return dist;
}

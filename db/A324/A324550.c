/*
 * A324550  Primes written in primorial base (A049345).
 * https://oeis.org/A324550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A049345(A000040(n))
 * Range:     u8 n <= 7, u16 n <= 228, u32 n <= 317, u64 n <= 317,
 *            u128 n <= 317, big any n
 * Helpers:   digit_len, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1; for safety */

    /* 1. obtain nth prime */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* 2. build primorials 1, 2, 2*3, 2*3*5, … until > p */
    const int MAX_K = 20;                      /* more than enough for n <= 10000 */
    seq_t primorial[MAX_K];
    primorial[0] = 1;                          /* 0# = 1 */
    int k = 1;
    while (1) {
        if (k >= MAX_K) break;                 /* safety guard */

        seq_t prime_k = (seq_t)get_prime(k);
        primorial[k] = primorial[k - 1] * prime_k;
        if (primorial[k] > p) break;
        ++k;
    }
    int K = k - 1;                             /* highest index with primorial <= p */

    /* 3. compute primorial base digits for p */
    int digit_val[MAX_K];
    seq_t remainder = p;
    for (int i = K; i >= 0; --i) {
        seq_t base = primorial[i];
        seq_t d = remainder / base;
        remainder = remainder % base;
        digit_val[i] = (int)d;                 /* d < next prime, fits in int */
    }

    /* 4. assemble decimal representation of the digits */
    seq_t result = 0;
    for (int i = K; i >= 0; --i) {
        seq_t d = (seq_t)digit_val[i];
        idx_t len = digit_len(d);              /* number of decimal digits of d */
        result = result * pow10(len) + d;
    }

    return result;
}

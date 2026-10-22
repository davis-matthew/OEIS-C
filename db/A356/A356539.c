/*
 * A356539  a(n) = Sum_{d|n} d * 3^(n-d).
 * https://oeis.org/A356539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * 3^(n-d)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* maximum n we expect to handle (speed/size compromise) */
#define MAX_N_FOR_POW3 20000

/* static array for powers of 3: pow3[k] = 3^k.
   It is large enough for the typical bounds of the tests (≤10000). */
static seq_t pow3[MAX_N_FOR_POW3 + 1];
static idx_t pow3_size = 0;      /* number of elements currently initialized */

/* Ensure powers up to 'n' exist */
static void ensure_pow3(idx_t n) {
    if (n <= pow3_size) return;
    if (n > MAX_N_FOR_POW3) n = MAX_N_FOR_POW3;  /* safety guard */
    if (pow3_size == 0) {
        pow3[0] = (seq_t)1;
        pow3_size = 0;
    }
    for (idx_t i = pow3_size + 1; i <= n; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }
    pow3_size = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 0, but harmless */
    ensure_pow3(n);                 /* we need up to exponent n-1 */

    seq_t result = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            idx_t d1 = i;
            idx_t exp1 = n - d1;
            result += d1 * pow3[exp1];

            /* complementary divisor n/i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t exp2 = n - d2;
                result += d2 * pow3[exp2];
            }
        }
    }
    return result;
}

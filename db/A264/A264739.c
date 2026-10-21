/*
 * A264739  Characteristic function of A002202, the range of Euler's totient
 *          function A000010.
 * https://oeis.org/A264739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a value of Euler's totient function φ, * else
 *            0. The value is zero for all n not in the range of φ
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_N 200000          /* pre‑compute totients up to this limit */

/* arrays large enough to hold all φ values up to MAX_N */
static idx_t phi_array[MAX_N + 1];
static unsigned char is_totient[MAX_N + 1];
static bool initialized = false;

/* one‑time initialization of phi_array and is_totient */
static void init_totients(void) {
    for (idx_t i = 0; i <= MAX_N; ++i) {
        phi_array[i] = i;
        is_totient[i] = 0;
    }
    for (idx_t p = 2; p <= MAX_N; ++p) {
        if (phi_array[p] == p) {           /* p is prime */
            for (idx_t j = p; j <= MAX_N; j += p) {
                phi_array[j] -= phi_array[j] / p;
            }
        }
    }
    for (idx_t m = 1; m <= MAX_N; ++m) {
        is_totient[phi_array[m]] = 1;
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_totients();

    /* n == 0 is outside the_DEFINED offset; return 0 */
    if (n == 0 || n > MAX_N) return (seq_t)0;

    return (seq_t)is_totient[n];
}

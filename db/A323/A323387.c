/*
 * A323387  Primes whose digits are distinct square digits, i.e., consisting
 *          of only digits 0, 1, 4, 9.
 * https://oeis.org/A323387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   cmp_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all permutations of the digits 0, 1, 4, 9
   with distinct digits (no leading zero), testing each for primality. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/cmp_seq_t.h"

static const int DIGITS[4] = {0, 1, 4, 9};
static seq_t primes[64];
static size_t prime_count = 0;
static bool initialized = false;

/* Recursive generation of distinct‑digit numbers. */
static void generate(seq_t current, int depth, int used_mask) {
    if (depth >= 2) {
        if (is_prime_big(current)) {
            primes[prime_count++] = current;
        }
    }
    if (depth == 4) return;
    for (int i = 0; i < 4; ++i) {
        if (used_mask & (1 << i)) continue;        /* already used */
        int d = DIGITS[i];
        if (depth == 0 && d == 0) continue;        /* leading zero disallowed */
        generate(current * 10 + d, depth + 1, used_mask | (1 << i));
    }
}

/* One‑time initialization of the sorted prime list. */
static void init(void) {
    if (initialized) return;
    generate((seq_t)0, 0, 0);
    qsort(primes, prime_count, sizeof(seq_t), cmp_seq_t);
    initialized = true;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n) {
    init();
    if (n == 0) return (seq_t)0;                     /* undefined for n < 1 */
    if ((idx_t)prime_count >= n) return primes[(size_t)(n - 1)];
    return (seq_t)0;                                 /* out of range → 0   */
}

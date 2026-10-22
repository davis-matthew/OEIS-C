/*
 * A322978  Number of even divisors d of 2n such that d-1 is prime.
 * https://oeis.org/A322978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of even divisors d of 2n such that d‑1 is
 *            prime
 * Range:     u8 n <= 8393, u16 n <= 8393, u32 n <= 8393, u64 n <= 8393,
 *            u128 n <= 8393, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Prime table up to 20000 (enough for n <= 10000). */
static bool prime_table[20001];
static int prime_table_initialized = 0;

static void init_prime_table(void) {
    if (prime_table_initialized) return;
    for (int i = 0; i <= 20000; ++i) prime_table[i] = true;
    prime_table[0] = prime_table[1] = false;
    for (int p = 2; p * p <= 20000; ++p) {
        if (prime_table[p]) {
            for (int mult = p * p; mult <= 20000; mult += p)
                prime_table[mult] = false;
        }
    }
    prime_table_initialized = 1;
}

seq_t a(idx_t n) {
    if (!prime_table_initialized) init_prime_table();

    seq_t count = 0;
    idx_t m = n;

    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t t = i;
            if (prime_table[(uint64_t)(2 * t) - 1])
                count++;

            idx_t t2 = m / i;
            if (t2 != t) {
                if (prime_table[(uint64_t)(2 * t2) - 1]) count++;
            }
        }
    }

    return count;
}

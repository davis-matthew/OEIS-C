/*
 * A253706  Primes in the 8th-order Fibonacci numbers A079262.
 * https://oeis.org/A253706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes in the 8th-order Fibonacci numbers A079262
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static seq_t last[8];

    if (!init) {
        /* first eight 8th‑order Fibonacci terms a(0)..a(7) */
        last[0] = 0;
        last[1] = 1;
        last[2] = 1;
        last[3] = 2;
        last[4] = 4;
        last[5] = 8;
        last[6] = 16;
        last[7] = 32;

        /* allocate cache for primes found */
        primes_cap = 128;
        primes = seq_alloc(primes_cap);
        primes_len = 0;

        /* check for primes among these initial terms */
        for (int i = 0; i < 8; ++i) {
            if (is_prime_big(last[i])) {
                if (primes_len >= primes_cap) {
                    size_t new_cap = primes_cap * 2;
                    seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = last[i];
            }
        }
        init = true;
    }

    /* Generate further 8th‑order Fibonacci numbers until we have n primes */
    while (primes_len < n) {
        seq_t sum = 0;
        for (int i = 0; i < 8; ++i) sum += last[i];

        if (is_prime_big(sum)) {
            if (primes_len >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = sum;
        }

        /* slide the window: drop the oldest term, append the new term */
        for (int i = 0; i < 7; ++i)
            last[i] = last[i + 1];
        last[7] = sum;
    }

    return primes[n - 1];
}

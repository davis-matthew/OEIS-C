/*
 * A295451  a(n) = (14*n)!*n!/((7*n)!*(5*n)!*(3*n)!).
 * https://oeis.org/A295451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n)!*n! / ((7*n)!*(5*n)!*(3*n)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   count_factor, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

#define MAX_PRIME_LIMIT 140000  /* enough for n ≤ 10000 (14*n) */

static int  primes[MAX_PRIME_LIMIT/10 + 10];
static size_t prime_count = 0;

/* simple sieve to fill primes[] up to MAX_PRIME_LIMIT */
static void init_primes(void) {
    if (prime_count > 0) return;
    char sieve[MAX_PRIME_LIMIT + 1];
    memset(sieve, 1, sizeof(sieve));
    sieve[0] = sieve[1] = 0;
    for (int i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    for (int i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (sieve[i]) primes[prime_count++] = i;
}

/* exponentiation for seq_t; assumes seq_pow is declared in utils */
#include "../utils/seq_pow.h"
#include "../utils/count_factor.h"

/* compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 1;

    init_primes();

    seq_t result = 1;
    idx_t limit = 14 * n;

    for (size_t i = 0; i < prime_count && primes[i] <= limit; ++i) {
        idx_t p   = (idx_t)primes[i];
        idx_t exp =  count_factor(14 * n, p)
                    + count_factor(n,      p)
                    - count_factor(7  * n, p)
                    - count_factor(5  * n, p)
                    - count_factor(3  * n, p);
        if (exp == 0) continue;
        result *= seq_pow((seq_t)p, exp);
    }
    return result;
}

/*
 * A254929  Fundamental positive solution y = y2(n) of the second class of the
 *          Pell equation x^2 - 2*y^2 = A007522(n), n>=1 (primes congruent to
 *          7 mod 8).
 * https://oeis.org/A254929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the larger of the two positive y‑solutions of the Pell
 *            equation x^2 - 2*y^2 = p_n, where p_n is the nth prime congruent
 *            to 7 modulo 8. The two solutions are obtained by searching y
 *            from 1 upward and testing whether p_n + 2*y^2 is a perfect
 *            square. The second found y (i.e. the larger one) i...
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/isqrt_seq.h"

/* Upper limit for the sieve (sufficient for 10,000 primes ≡7 (mod 8) ). */
#define SIEVE_LIMIT 1000000
#define MAX_PRIMES7 20000  /* number of primes ≡7 (mod 8) up to SIEVE_LIMIT */

static uint8_t sieve[SIEVE_LIMIT + 1];
static uint32_t primes7[MAX_PRIMES7];
static size_t prime7_count = 0;
static int primes_initialized = 0;

static void init_primes(void)
{
    if (primes_initialized) return;
    memset(sieve, 0, sizeof(sieve));
    sieve[0] = sieve[1] = 1;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 1;
        }
    }
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i] && ((i & 7) == 7)) {
            if (prime7_count < MAX_PRIMES7)
                primes7[prime7_count++] = (uint32_t)i;
        }
    }
    primes_initialized = 1;
}

seq_t a(idx_t n)
{
    /* Ensure primes list is ready. */
    init_primes();

    /* The problem guarantees n>=1; 0‑based indexing for array. */
    if (n < 1 || (size_t)(n - 1) >= prime7_count) {
        /* Out of bounds; return 0 to signal failure. */
        return (seq_t)0;
    }

    uint32_t p = primes7[(size_t)(n - 1)];   /* nth prime ≡7 (mod 8) */

    idx_t y1 = 0, y2 = 0;
    for (idx_t y = 1; ; ++y) {
        seq_t yy = (seq_t)y;
        seq_t val = (seq_t)p + (seq_t)2 * yy * yy;
        seq_t x   = isqrt_seq(val);
        if (x * x == val) {
            if (y1 == 0)
                y1 = y;
            else {
                y2 = y;
                break;
            }
        }
    }
    return (seq_t)y2;
}

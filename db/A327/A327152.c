/*
 * A327152  r values of Triphosian primes.
 * https://oeis.org/A327152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if ⌊(n‑1)/3⌋ is odd; otherwise * a(n) = the
 *            ((⌊(n‑1)/6⌋)-th element of the sequence * (2,5,11,17,23,29,…) –
 *            i.e. the primes congruent to 5 (mod 6) * with 2 as the first
 *            element
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t primes[2048];
static size_t prime_count = 0;
static uint64_t next_candidate = 5;

/* Simple deterministic primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n <= 3) return true;
    if ((n & 1) == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Ensure primes[0..k] are available */
static void ensure_primes(size_t k)
{
    if (prime_count == 0) {
        primes[prime_count++] = (seq_t)2;          /* first special prime */
        next_candidate = 5;                        /* next candidate of form 6k+5 */
    }
    while (prime_count <= k) {
        uint64_t candidate = next_candidate;
        for (;;) {
            if (is_prime_uint64(candidate)) {
                primes[prime_count++] = (seq_t)candidate;
                next_candidate = candidate + 6;    /* advance to next 6k+5 candidate */
                break;
            }
            candidate += 6;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, return 0 */
    idx_t g = (n - 1) / 3;
    if (g & 1) return (seq_t)0;          /* odd block → 0 */
    idx_t k = g / 2;                      /* index in the prime sequence */
    ensure_primes((size_t)k);
    return primes[(size_t)k];
}

/*
 * A275732  One-based positions of 1-digits in the factorial base
 *          representation of n are converted to primes with those indices,
 *          then multiplied together.
 * https://oeis.org/A275732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of prime(p) for every position p (1‑based from
 *            the right) whose factorial‑base digit in n equals 1; a(n)=1 if
 *            no such digit exists
 * Range:     u8 n <= 128, u16 n <= 5888, u32 n <= 9614, u64 n <= 9614,
 *            u128 n <= 9614, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t primes[2000];
static size_t prime_count = 0;

static void ensure_prime_index(size_t idx)
{
    while (prime_count <= idx) {
        uint32_t candidate = prime_count ? primes[prime_count - 1] + 1 : 2;
        bool ok;
        do {
            ok = true;
            for (size_t j = 0; j < prime_count; ++j) {
                uint64_t pj = primes[j];
                if (pj * pj > candidate) break;
                if (candidate % pj == 0) { ok = false; break; }
            }
            if (!ok) ++candidate;
        } while (!ok);
        primes[prime_count++] = candidate;
    }
}

seq_t a(idx_t n)
{
    seq_t product = 1;
    idx_t tmp = n;
    size_t pos = 1;               /* position counted from the right, starts at 1 */

    while (tmp > 0) {
        uint64_t divisor = (uint64_t)pos + 1ULL;   /* next factorial base divisor */
        uint64_t digit   = (uint64_t)tmp % divisor;
        tmp /= divisor;

        if (digit == 1ULL) {
            ensure_prime_index(pos - 1);           /* primes array is 0‑based */
            product *= (seq_t)primes[pos - 1];
        }
        ++pos;
    }
    return product;
}

/*
 * A286468  Run lengths in the binary expansion of n are one more than the
 *          exponents of corresponding primes in the prime factorization of
 *          a(n).
 * https://oeis.org/A286468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over run-length groups of
 *            prime(index)^(length-1), where run-lengths are counted from the
 *            least‑significant binary bit of n
 * Range:     u8 n <= 120, u16 n <= 996, u32 n <= 8740, u64 n <= 8740,
 *            u128 n <= 8740, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* A small prime table – enough for any run‑length count up to 128. */
static bool primes_inited = false;
static uint32_t primes[200];
static size_t primes_used = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    const size_t limit = 2000;          /* enough to generate >200 primes */
    bool flags[limit + 1];
    for (size_t i = 0; i <= limit; ++i) flags[i] = true;
    flags[0] = flags[1] = false;
    for (size_t i = 2; i <= limit && primes_used < 200; ++i) {
        if (!flags[i]) continue;
        primes[primes_used++] = (uint32_t)i;
        for (size_t j = i * i; j <= limit; j += i) flags[j] = false;
    }
}

/* Compute a(n) for the OEIS sequence A286468. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n≥1, but guard */
    init_primes();

    seq_t result = 1;
    idx_t remaining = n;
    int current_bit = remaining & 1;
    idx_t count = 1;
    remaining >>= 1;
    size_t prime_index = 0;

    while (remaining != 0) {
        int new_bit = remaining & 1;
        if (new_bit == current_bit) {
            ++count;
        } else {
            /* process the run that just ended */
            if (count > 1) {
                seq_t p = (seq_t)primes[prime_index];
                for (idx_t exp = 0; exp < count - 1; ++exp) {
                    result *= p;
                }
            }
            /* advance to next prime for the new run */
            ++prime_index;
            current_bit = new_bit;
            count = 1;
        }
        remaining >>= 1;
    }
    /* process the final run */
    if (count > 1) {
        seq_t p = (seq_t)primes[prime_index];
        for (idx_t exp = 0; exp < count - 1; ++exp) {
            result *= p;
        }
    }
    return result;
}

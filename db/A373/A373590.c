/*
 * A373590  Numbers whose number of prime factors (with multiplicity) is a
 *          multiple of 3, and all of them are of the type 3m+1 (in A002476).
 * https://oeis.org/A373590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer composed only of primes ≡1 (mod 3) whose
 *            total number of prime factors (with multiplicity) is a multiple
 *            of 3. Computed by linear search with trial division
 * Range:     u8 n <= 1, u16 n <= 442, u32 n <= 9210, u64 n <= 9210,
 *            u128 n <= 9210, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes.h"

static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_ready = false;

static bool in_sequence(idx_t x)
{
    if (x == 0) return false;
    idx_t temp = x;
    idx_t omega = 0;
    bool ok = true;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            if (p % 3 != 1) ok = false;
            while (temp % p == 0) {
                ++omega;
                temp /= p;
            }
        }
    }
    if (temp > 1) {
        if (temp % 3 != 1) ok = false;
        ++omega;
    }
    return ok && ((omega % 3) == 0);
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    if (!primes_ready) {
        idx_t limit = 1000000; /* sufficient for n up to several 10⁴ */
        build_primes(limit, &primes, &primes_cnt);
        primes_ready = true;
    }
    idx_t count = 0;
    for (idx_t candidate = 1;; ++candidate) {
        if (in_sequence(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
    }
}

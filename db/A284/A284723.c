/*
 * A284723  Smallest odd prime that is relatively prime to n.
 * https://oeis.org/A284723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime p such that n % p != 0
 * Range:     u8 n <= 9290, u16 n <= 9290, u32 n <= 9290, u64 n <= 9290,
 *            u128 n <= 9290, big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (!primes_ready) {
        size_t cnt;
        uint64_t *arr;
        /* Build a list of odd primes up to 2000 (more than enough for n up to 2^64) */
        build_primes_upto(2000, &arr, &cnt);
        primes = arr;
        prime_cnt = cnt;
        primes_ready = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but return 0 safely */

    init_primes();

    /* Scan the pre‑computed odd primes, skipping the prime 2. */
    for (size_t i = 1; i < prime_cnt; ++i) {
        uint64_t p = primes[i];          /* odd prime */
        if (n % p != 0) {
            return (seq_t)p;
        }
    }

    /* Fallback – generate odd primes beyond the pre‑computed list
       (theoretically unreachable for our limits but kept for safety). */
    uint64_t candidate = primes[prime_cnt - 1] + 2;
    while (1) {
        if (is_prime_small(candidate)) {
            if (n % candidate != 0) {
                return (seq_t)candidate;
            }
        }
        candidate += 2;
    }
}

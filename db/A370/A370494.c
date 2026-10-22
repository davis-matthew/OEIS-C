/*
 * A370494  Oblong numbers of the form (k-1)*k where k is the product of an
 *          odd number of distinct primes.
 * https://oeis.org/A370494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (k-1)*k where k is squarefree and has an odd number of
 *            distinct prime factors
 * Range:     u8 n <= 6, u16 n <= 79, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"

/* primes up to 224 are enough to factor all k needed for n ≤ 10 000 */
static bool primes_ready = false;
static uint64_t *small_primes = NULL;
static size_t small_primes_cnt = 0;

static void init_small_primes(void)
{
    if (primes_ready) return;
    /* 224² = 50176, well above the needed range for the first 10 000 terms */
    idx_t limit = 224;
    build_primes_upto(limit, &small_primes, &small_primes_cnt);
    primes_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_small_primes();

    idx_t found = 0;
    for (idx_t k = 2; ; ++k) {
        uint64_t tmp = k;
        uint64_t omega = 0;
        bool squarefree = true;

        for (size_t i = 0; i < small_primes_cnt && small_primes[i] * small_primes[i] <= tmp; ++i) {
            uint64_t p = small_primes[i];
            if (tmp % p == 0) {
                int exp = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++exp;
                    if (exp > 1) {        /* not squarefree */
                        squarefree = false;
                        break;
                    }
                }
                if (!squarefree) break;
                ++omega;
            }
        }

        if (squarefree && tmp > 1) ++omega;   /* remaining prime factor */

        if (squarefree && (omega & 1ULL)) {   /* odd number of distinct primes */
            ++found;
            if (found == n) {
                seq_t kk = (seq_t)k;
                return kk * (kk - (seq_t)1);
            }
        }
    }
}

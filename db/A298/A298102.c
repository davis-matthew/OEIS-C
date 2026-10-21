/*
 * A298102  The first of five consecutive integers the sum of which is equal
 *          to the sum of five consecutive prime numbers.
 * https://oeis.org/A298102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 267, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest integer m such that 5*m+10 equals the sum of
   five consecutive primes.  The implementation scans windows of
   five consecutive primes, collects the corresponding m values
   (in ascending order), and returns the n‑th element.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc_new_array.h"

/* static workspace */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static seq_t *ms_seq = NULL;
static size_t ms_cap = 0, ms_cnt = 0;
static bool initialized = false;

/* initialise primes and the m‑sequence once */
static void init(void)
{
    if (initialized) return;

    /* 1 000 000 is sufficient to obtain >15 000 ms, well above the
       needed 10 000 terms.                                                    */
    const idx_t prime_limit = 1000000ULL;

    build_primes_upto(prime_limit, &primes, &primes_cnt);

    ms_cap = primes_cnt;          /* safe upper bound                   */
    ms_seq = seq_alloc_new_array(ms_cap);

    size_t pos = 0;
    if (primes_cnt >= 5) {
        for (size_t i = 0; i + 4 < primes_cnt; ++i) {
            seq_t sum = 0;
            sum += (seq_t)primes[i];
            sum += (seq_t)primes[i + 1];
            sum += (seq_t)primes[i + 2];
            sum += (seq_t)primes[i + 3];
            sum += (seq_t)primes[i + 4];

            if ((sum % (seq_t)5) == 0) {
                seq_t m = (sum - (seq_t)10) / (seq_t)5;
                ms_seq[pos++] = m;
            }
        }
    }
    ms_cnt = pos;
    initialized = true;
}

seq_t a(idx_t n)
{
    init();                           /* ensure data is ready             */
    if (n < 1 || (size_t)n > ms_cnt) return (seq_t)0;
    return ms_seq[(size_t)n - 1];
}

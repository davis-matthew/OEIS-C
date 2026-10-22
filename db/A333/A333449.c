/*
 * A333449  a(n) = Sum_{k=1..n} prime(floor(n/k)).
 * https://oeis.org/A333449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(floor(n/k))
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t *prime_cache = NULL;   /* array of primes (1‑based) */
static size_t prime_cache_cnt = 0;     /* number of primes stored */

/* Ensure that prime_cache contains at least 'need' primes.  */
static void ensure_primes(idx_t need)
{
    if (prime_cache_cnt >= need)
        return;

    /* Estimate an upper bound for the nth prime.
       For n >= 6, n*(ln n + ln ln n) < p_n < n*(ln n + ln ln n) + 3.   */
    double dn = (double)need;
    size_t limit = (need < 6) ? 15
                              : (size_t)(dn * (log(dn) + log(log(dn)))) + 3;
    /* Increase the limit until we have enough primes.  */
    while (1) {
        /* Allocate a sieve array.  */
        size_t i, j;
        uint8_t *is_composite = (uint8_t*)calloc(limit + 1, 1);
        if (!is_composite) abort();

        size_t cnt = 0;
        for (i = 2; i <= limit; ++i) {
            if (!is_composite[i]) {
                ++cnt;
                if (cnt >= need) break; /* enough primes */
                if (i <= limit / i) {
                    for (j = i * i; j <= limit; j += i)
                        is_composite[j] = 1;
                }
            }
        }

        /* Free previous cache and allocate new one if sufficient.  */
        if (cnt >= need) {
            uint64_t *new_cache = (uint64_t*)malloc(cnt * sizeof(uint64_t));
            if (!new_cache) abort();
            size_t k = 0;
            for (i = 2; i <= limit && k < cnt; ++i) {
                if (!is_composite[i]) {
                    new_cache[k++] = i;
                }
            }
            free(prime_cache);
            prime_cache = new_cache;
            prime_cache_cnt = cnt;
            free(is_composite);
            break;
        }
        /* Not enough primes: double the limit and retry.  */
        free(is_composite);
        limit *= 2;
    }
}

/* Main function for the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but safe return 0 */
    ensure_primes(n);     /* guarantees prime_cache has at least n primes */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        idx_t q1 = n / d;
        idx_t q2 = n / (d + 1);
        idx_t cnt = q1 - q2;        /* number of k with floor(n/k) == d */
        seq_t prime_val = (seq_t)prime_cache[d - 1];
        sum += prime_val * (seq_t)cnt;
    }
    return sum;
}

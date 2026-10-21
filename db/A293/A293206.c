/*
 * A293206  a(n) = prime(n) + prime(n+1) * prime(n+2).
 * https://oeis.org/A293206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + prime(n+1) * prime(n+2)
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* static storage for primes */
static idx_t prime_limit = 0;          /* current sieve upper bound   */
static uint64_t *prime_array = NULL;   /* list of primes (0‑based)    */
static size_t prime_count = 0;         /* number of primes stored     */

/* Ensure we have at least 'need' primes (1‑indexed).  If not, grow the sieve. */
static void ensure_primes(idx_t need)
{
    if (prime_count >= need) return;      /* already enough */

    /* Upper bound for the need‑th prime.  Rosser's theorem: p_n < n(ln n + ln ln n)   */
    idx_t limit;
    if (need < 6) {
        limit = 15;
    } else {
        double dn = (double)need;
        double U = dn * (log(dn) + log(log(dn))) + 3.0 * dn;
        limit = (idx_t)U;
    }

    /* Allocate composite flag array */
    size_t sz = (size_t)limit + 1;
    bool *is_composite = (bool *)calloc(sz, sizeof(bool));
    if (!is_composite) return;           /* should not happen */

    /* Temporary array to hold primes */
    uint64_t *new_primes = (uint64_t *)malloc(sz * sizeof(uint64_t));
    if (!new_primes) { free(is_composite); return; }

    size_t new_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            new_primes[new_count++] = (uint64_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }

    free(is_composite);

    /* Replace old prime list */
    if (prime_array) free(prime_array);
    prime_array = new_primes;
    prime_count = new_count;
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n=0, guard */

    /* Ensure we have primes up to the (n+2)-th prime */
    ensure_primes(n + 2);

    /* Compute a(n) = prime(n) + prime(n+1) * prime(n+2) */
    seq_t p_n   = (seq_t)prime_array[n - 1];   /* prime(n)   */
    seq_t p_np  = (seq_t)prime_array[n];      /* prime(n+1) */
    seq_t p_npp = (seq_t)prime_array[n + 1];  /* prime(n+2) */

    return p_n + p_np * p_npp;
}

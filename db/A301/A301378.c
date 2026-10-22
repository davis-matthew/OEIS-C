/*
 * A301378  a(n) = 10*A007605(n) - 9*A007652(n).
 * https://oeis.org/A301378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, less
 * Formula:   a(n) = (sum of all digits of the n-th prime except the last
 *            one)*10 * + the last digit of that prime * The function
 *            generates primes up to the required index using a simple *
 *            sieve. It caches the primes so repeated calls are fast
 * Range:     u8 n <= 1116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t prime_cache_count = 0;   /* number of primes cached */

/* Generate primes up to at least `need` primes.  The sieve limit is
 * increased until enough primes are found.  The sieve uses an array of
 * unsigned char to save memory. */
static void build_primes_upto_idx(idx_t need)
{
    if (prime_cache != NULL && prime_cache_count >= (size_t)need)
        return;                              /* already have enough */

    /* Free previous cache */
    free(prime_cache);
    prime_cache = NULL;
    prime_cache_count = 0;

    /* Rough estimate of the n-th prime: use n*15 + 100 as an upper
     * bound for n up to about 20 000.  If this is insufficient the
     * loop expands the limit. */
    size_t lim = (size_t)need * 15 + 100;
    if (lim < 100) lim = 100;

    while (1) {
        /* Sieve */
        unsigned char *is_prime = (unsigned char*)malloc(lim + 1);
        if (!is_prime) return;  /* allocation failure – give up silently */
        for (size_t i = 0; i <= lim; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= lim; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= lim; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Count primes */
        size_t estimated = 0;
        for (size_t i = 2; i <= lim; ++i)
            if (is_prime[i]) ++estimated;

        /* Allocate array to hold primes */
        uint64_t *primes = (uint64_t*)malloc(sizeof(uint64_t) * estimated);
        if (!primes) {
            free(is_prime);
            return;
        }

        /* Copy primes into array */
        size_t idx = 0;
        for (size_t i = 2; i <= lim && idx < estimated; ++i) {
            if (is_prime[i]) {
                primes[idx++] = (uint64_t)i;
            }
        }

        free(is_prime);

        if (idx >= (size_t)need) {          /* we have enough */
            prime_cache = primes;
            prime_cache_count = idx;
            break;
        }

        /* Not enough primes – grow the limit and try again */
        free(primes);
        lim = lim * 2;   /* double the limit */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n=0 */

    build_primes_upto_idx(n);                /* ensure we have nth prime */

    uint64_t p = prime_cache[n - 1];          /* nth prime, 1‑based */
    uint64_t last = p % 10;                   /* last digit */
    uint64_t rest = p / 10;                   /* all but last digit */

    /* Sum of digits of `rest` */
    uint64_t sum = 0;
    while (rest > 0) {
        sum += rest % 10;
        rest /= 10;
    }

    /* Construct a(n) = sum*10 + last */
    seq_t result = (seq_t)sum * (seq_t)10 + (seq_t)last;
    return result;
}

/*
 * A373158  Fully additive with a(p) = p# for prime p, where x# is the
 *          primorial A034386(x).
 * https://oeis.org/A373158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∑_{p^e‖n} e · P(p)   where P(p) = ∏_{q≤p, q prime} q
 *   (P(p) is the primorial of p).  The sequence is completely additive
 *   with a(p) = P(p) for prime p.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached primes and primorials up to the largest n requested */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static seq_t *primorial_of_prime = NULL;   /* primorial_of_prime[p] = P(p) for prime p */
static size_t primorial_cap = 0;

/* Build/extend tables to guarantee support for all numbers up to max_n */
static void ensure_limit(idx_t max_n)
{
    if (max_n < 2) return;                     /* nothing to do for n < 2 */

    if (max_n < (idx_t)primorial_cap) return;  /* already sufficient */

    /* free old tables if they exist */
    if (primes) free(primes);
    if (primorial_of_prime) free(primorial_of_prime);
    primes = NULL;
    primorial_of_prime = NULL;
    primes_cnt = 0;
    primes_cap = 0;
    primorial_cap = 0;

    /* allocate new tables sized for max_n */
    size_t limit = (size_t)max_n;

    /* Rough estimate of number of primes ≤ limit: ≤ limit / log(limit) < limit/2 for limit≥4 */
    primes_cap = limit / 2 + 1;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));

    primorial_cap = limit + 1;
    primorial_of_prime = (seq_t*)calloc(primorial_cap, sizeof(seq_t));

    /* Sieve of Eratosthenes to list primes ≤ limit */
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            /* store prime */
            if (primes_cnt == primes_cap) {
                /* double capacity if needed */
                primes_cap *= 2;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;

            /* mark multiples */
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }
    }

    /* compute primorials: P(p) = product of all primes ≤ p */
    seq_t curr = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        curr *= (seq_t)primes[i];
        primorial_of_prime[primes[i]] = curr;
    }

    free(is_prime);
}

/*
 * Return a(n) as defined above.  The function is thread‑safe only on the
 * assumption that calls are serialized; static memory is reused for
 * efficiency but may be recomputed if a larger n is requested.
 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* defined to be 0 for n < 1 */

    ensure_limit(n);              /* guarantee primes and primorials up to n exist */

    idx_t temp = n;
    seq_t sum = 0;

    while (temp > 1) {
        uint32_t p = 0;

        /* trial divide by cached primes */
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t pr = primes[i];
            if ((uint64_t)pr * (uint64_t)pr > temp) break;  /* no further factors possible */
            if (temp % pr == 0) {
                p = pr;
                break;
            }
        }

        if (p == 0) { /* temp itself is prime */
            p = (uint32_t)temp;
        }

        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* add e * P(p) */
        sum += (seq_t)e * primorial_of_prime[p];
    }

    return sum;
}

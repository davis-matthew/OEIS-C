/*
 * A362325  Table read by antidiagonals: T(n,k) = number of numbers <= n that
 *          can be fully factored using the first k prime numbers.
 * https://oeis.org/A362325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) read by antidiagonals of the table where T(i,j) =
 *            number of integers ≤ i that can be fully factored only with the
 *            first j primes. The mapping from index n to (i,j) is: let t be
 *            the smallest integer with t*(t+1)/2 ≥ n, then i = t - (n -
 *            (t-1)*t/2 - 1), j = (n - (t-1)*t/2 - 1) + 1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int primes_precomputed = 0;
static size_t primes_count = 0;
static uint64_t primes[2263]; /* 2262 primes up to 20000 */

/* Build the list of the first 2262 primes (up to 20000). */
static void init_primes(void) {
    if (primes_precomputed) return;
    const size_t LIMIT = 20000;
    unsigned char *sieve = (unsigned char *)malloc(LIMIT + 1);
    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) sieve[j] = 0;
        }
    }
    primes_count = 0;
    for (size_t i = 2; i <= LIMIT && primes_count < 2262; ++i)
        if (sieve[i]) primes[primes_count++] = i;
    free(sieve);
    primes_precomputed = 1;
}

/* Smallest prime factor array for numbers up to current limit. */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

static void ensure_spf(idx_t limit) {
    if (limit <= spf_limit) return;
    idx_t *new_spf = (idx_t *)malloc(sizeof(idx_t) * (limit + 1));
    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    new_spf[0] = 0;
    new_spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }
    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Count numbers ≤ i that have all prime factors ≤ the j‑th prime. */
static seq_t count_T(idx_t i, idx_t j) {
    if (i == 0) return 0;
    seq_t cnt = 0;
    ensure_spf(i);
    for (idx_t x = 1; x <= i; ++x) {
        if (x == 1) {
            ++cnt;
            continue;
        }
        idx_t temp = x;
        int ok = 1;
        while (temp > 1) {
            idx_t pf = spf[temp];
            if (pf > primes[(size_t)(j - 1)]) { /* j ≥ 1 */
                ok = 0;
                break;
            }
            temp /= pf;
        }
        if (ok) ++cnt;
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety, though offset is 1 */
    init_primes();

    /* Find smallest t with t*(t+1)/2 ≥ n.  t = s-1. */
    idx_t t = 0;
    while (t * (t + 1) / 2 < n) ++t;

    idx_t prev_total = (t - 1) * t / 2;
    idx_t p = n - prev_total - 1; /* zero‑based offset within diagonal */

    idx_t i = t - p;
    idx_t j = p + 1; /* j = p+1 works because j ranges 1..t */

    return count_T(i, j);
}

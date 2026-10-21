/*
 * A248891  Number of primes p such that p+2 is prime and A001359(n) < p <
 *          A001359(n)^(1+1/n).
 * https://oeis.org/A248891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248891: a(n) = #{p prime | p+2 prime, A001359(n) < p < A001359(n)^(1+1/n)} */
/* A001359 is the sequence of lower members of twin primes: 3,5,11,17,29,41,59,...      */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define LIMIT 2500000   /* primes up to this limit suffice for n ≤ 10000 */

static unsigned char *is_prime = NULL;
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t *twin_lower = NULL;
static size_t twin_cnt = 0;
static bool init_done = false;

static void init(void)
{
    if (init_done) return;

    /* ---- sieve ---- */
    is_prime = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
        }
    }

    /* ---- primes list ---- */
    primes_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) primes_cnt++;
    primes = (idx_t*)malloc(primes_cnt * sizeof(idx_t));
    size_t pos = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) primes[pos++] = (idx_t)i;

    /* ---- lower twin primes list ---- */
    twin_cnt = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p + 2 > LIMIT) break;
        if (is_prime[p + 2]) twin_cnt++;
    }
    twin_lower = (idx_t*)malloc(twin_cnt * sizeof(idx_t));
    size_t tpos = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p + 2 > LIMIT) break;
        if (is_prime[p + 2]) twin_lower[tpos++] = p;
    }

    init_done = true;
}

/* binary search: first index where twin_lower[idx] > key */
static size_t next_superprime_idx(size_t key)
{
    size_t lo = 0, hi = twin_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (twin_lower[mid] <= key) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    init();

    if (n == 0) return 0;                      /* not used, offset starts at 1 */
    if (n > twin_cnt) return 0;                /* safety guard */

    idx_t base = twin_lower[n - 1];

    /* compute upper bound: base^(1+1/n) as a double */
    double exp = 1.0 + 1.0 / (double)n;
    double upper = pow((double)base, exp);

    size_t start = n;                       /* first twin > base */
    if (start > twin_cnt) return 0;

    seq_t count = 0;
    for (size_t i = start; i < twin_cnt; ++i) {
        idx_t p = twin_lower[i];
        if ((double)p >= upper) break;      /* strict inequality */
        /* p+2 must be prime – guaranteed by construction of twin_lower */
        count++;
    }
    return count;
}

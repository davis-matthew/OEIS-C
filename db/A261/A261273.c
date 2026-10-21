/*
 * A261273  a(n) is the (prime(n)+1)-th term of Stern-Brocot sequence,
 *          A002487.
 * https://oeis.org/A261273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = SternBrocot(1 + prime_n) Where SternBrocot(k) is Stern's
 *            diatomic sequence A002487(k)
 * Range:     u8 n <= 1212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* Simple sieve to generate at least n primes.  A bound of 20*n+100
   is safely above the 10 000th prime (104 729). */
static void ensure_primes(idx_t n)
{
    if (n <= primes_cnt) return;
    idx_t bound = n * 20 + 100;
    if (bound < 200) bound = 200;

    /* allocate boolean array for sieve */
    char *is_prime = (char*)calloc((size_t)bound + 1, 1);
    if (!is_prime) abort();          /* out of memory */
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i <= (size_t)bound; ++i) is_prime[i] = 1;

    for (size_t i = 2; i * i <= (size_t)bound; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)bound; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to reallocate */
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)bound; ++i)
        if (is_prime[i]) ++count;

    uint32_t *tmp = (uint32_t*)realloc(primes, count * sizeof(uint32_t));
    if (!tmp) abort();
    primes = tmp;
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)bound; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;
    primes_cnt = count;
    free(is_prime);
}

/* Stern's diatomic sequence cache */
static seq_t *stern = NULL;
static size_t stern_cap = 0;
static idx_t stern_max = 0;

static void ensure_stern(idx_t n)
{
    if (n <= stern_max) return;
    if (stern_cap < (size_t)(n + 1)) {
        stern_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(stern, stern_cap * sizeof(seq_t));
        if (!tmp) abort();
        stern = tmp;
    }

    /* Initialize known base values */
    stern[0] = 0;
    if (stern_cap > 1) stern[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1ULL) == 0)
            stern[i] = stern[i >> 1];
        else
            stern[i] = stern[(i - 1) >> 1] + stern[(i + 1) >> 1];
    }
    stern_max = n;
}

/* Main function requested by the exercise */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1, safe fallback */

    /* Get the n-th prime (1‑based) */
    ensure_primes(n);
    uint32_t p = primes[n - 1];
    idx_t k = (idx_t)p + 1ULL;       /* we need A002487(1+prime(n)) */

    /* Ensure Stern cache covers this index */
    ensure_stern(k);

    return stern[k];
}

/*
 * A289928  p-INVERT of (1,2,3,5,7,11,13,...); i.e., 1 and the primes
 *          (A008578), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in (1/(1 - S(x) - S(x)^2) - 1)/x with
 *            S(x) = x + 2x^2 + 3x^3 + 5x^4 + 7x^5 + … (1 followed by all
 *            primes). Equivalent recurrence used below: Let c[0] = 1, c[i] =
 *            i‑th prime for i ≥ 1. Define conv[k] = Σ_{i=1}^{k-1}
 *            c[i-1]·c[k-i-1] (for k ≥ 2, conv[0]=conv[1]=0). Let b[0]...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Prime cache ------------------------------------------------------------ */
static uint64_t *prime_cache = NULL;
static size_t   prime_cache_len = 0;
static size_t   prime_cache_cap = 0;

static void ensure_primes(size_t need)
/* Ensures that prime_cache contains at least 'need' primes (index 0 = 2). */
{
    if (need <= prime_cache_len)
        return;

    if (!prime_cache) {
        prime_cache_cap = 16;
        prime_cache = (uint64_t *)malloc(prime_cache_cap * sizeof(uint64_t));
        prime_cache[0] = 2;
        prime_cache_len = 1;
    }

    uint64_t candidate = prime_cache[prime_cache_len - 1] + 1;
    while (prime_cache_len < need) {
        bool is_prime = true;
        uint64_t r = (uint64_t)sqrt((double)candidate);
        for (size_t i = 0; i < prime_cache_len && prime_cache[i] <= r; ++i) {
            if (candidate % prime_cache[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cache_len >= prime_cache_cap) {
                prime_cache_cap *= 2;
                prime_cache = (uint64_t *)realloc(prime_cache, prime_cache_cap * sizeof(uint64_t));
            }
            prime_cache[prime_cache_len++] = candidate;
        }
        ++candidate;
    }
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n;      /* we need c[0..N]  -> N+1 elements */
    ensure_primes(N);          /* primes[0..N-1]  */

    /* Allocate working arrays */
    seq_t *c    = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *conv = (seq_t *)malloc((N + 2) * sizeof(seq_t)); /* indices up to N+1 */
    seq_t *b    = (seq_t *)malloc((N + 2) * sizeof(seq_t));

    /* c[0] = 1, c[i] = prime_i for i >= 1   (primes array is 0‑based) */
    c[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        c[i] = (seq_t)prime_cache[i - 1];

    /* conv[k] = Σ_{i=1}^{k-1} c[i-1] * c[k-i-1]   for k >= 2 */
    conv[0] = conv[1] = (seq_t)0;
    for (size_t k = 2; k <= N + 1; ++k) {
        seq_t sum = 0;
        for (size_t i = 1; i < k; ++i)
            sum += c[i - 1] * c[k - i - 1];
        conv[k] = sum;
    }

    /* b[0] = 1; compute b[1..N+1] */
    b[0] = (seq_t)1;
    for (size_t m = 1; m <= N + 1; ++m) {
        seq_t sum1 = 0;
        for (size_t i = 1; i <= m; ++i)
            sum1 += c[i - 1] * b[m - i];

        seq_t sum2 = 0;
        if (m >= 2) {
            for (size_t j = 2; j <= m; ++j)
                sum2 += conv[j] * b[m - j];
        }

        b[m] = sum1 + sum2;
    }

    seq_t result = b[N + 1];

    free(c);
    free(conv);
    free(b);
    return result;
}

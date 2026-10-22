/*
 * A358558  a(n) is the number of pairs (k,m) of positive integers with 1 <= k
 *          < m <= n such that gcd(k,m) = 2^t, t > 0.
 * https://oeis.org/A358558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{t≥1, 2^t ≤ n/2} ∑_{k=2}^{⌊n/2^t⌋} φ(k)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              φ denotes Euler's totient function. */
#include <stdint.h>
#include <stdlib.h>

static size_t maxN = 0;
static uint64_t *phi_arr = NULL;   /* φ values */
static seq_t   *pref_arr = NULL;   /* prefix sums of φ(k) for k≥2 */

static void recompute_phi_prefix(size_t new_maxN)
{
    size_t N = new_maxN;
    uint64_t *phi = (uint64_t *)realloc(phi_arr, (N + 1) * sizeof(uint64_t));
    seq_t   *pref = (seq_t   *)realloc(pref_arr, (N + 1) * sizeof(seq_t));
    if (!phi || !pref) { /* in the unlikely event of malloc failure */
        exit(EXIT_FAILURE);
    }
    phi_arr = phi;
    pref_arr = pref;

    /* Linear sieve for φ up to N */
    size_t *primes = (size_t *)malloc((N / 10 + 1) * sizeof(size_t));
    size_t primes_cnt = 0;
    size_t *is_comp = (size_t *)calloc(N + 1, sizeof(size_t));
    if (!primes || !is_comp) {
        exit(EXIT_FAILURE);
    }
    phi[1] = 1;
    for (size_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            phi[i] = i - 1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = primes[j];
            if (p * i > N) break;
            is_comp[p * i] = 1;
            if (i % p == 0) {
                phi[p * i] = phi[i] * p;
                break;
            } else {
                phi[p * i] = phi[i] * (p - 1);
            }
        }
    }
    free(primes);
    free(is_comp);

    /* Build prefix sums of φ(k) for k≥2 */
    pref[0] = 0;
    pref[1] = 0;
    for (size_t i = 2; i <= N; ++i) {
        pref[i] = pref[i - 1] + (seq_t)phi[i];
    }
    maxN = N;
}

/* ==========================================================================
   Main sequence function
   ========================================================================== */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* a(1) = 0, a(2) = 0 for completeness */

    size_t N = (size_t)n;
    if (N > maxN) {
        recompute_phi_prefix(N);
    }

    seq_t result = 0;
    /* Loop over powers of two d = 2^t, t ≥ 1, while d ≤ n/2 */
    for (size_t d = 2; d <= N / 2; d <<= 1) {
        size_t m = N / d;           /* floor(n / 2^t) */
        result += pref_arr[m];
    }
    return result;
}

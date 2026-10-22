/*
 * A328768  The first primorial based variant of arithmetic derivative:
 *          a(prime(i)) = A002110(i-1), where prime(i) = A000040(i), a(u*v) =
 *          a(u)*v + u*a(v), with a(0) = a(1) = 0.
 * https://oeis.org/A328768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{p^e‖n} e * a(p)/p * where a(p) = product of all
 *            primes < p (primorial of preceding prime index) * and
 *            a(0)=a(1)=0
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 58, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t   *primes  = NULL;       /* list of primes up to cached limit */
static seq_t      *primorials = NULL;   /* product of primes less than primes[i] */
static size_t     prime_cnt = 0;        /* number of primes stored */
static size_t     cache_limit = 0;      /* highest n for which primes are cached */

/* build primes and primorials up to 'limit' (inclusive) */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= cache_limit) return;

    /* free previous tables */
    free(primes);
    free(primorials);

    size_t sz = (size_t)(limit + 1);
    primes      = (uint64_t *)malloc(sz * sizeof(uint64_t));
    primorials  = (seq_t   *)malloc(sz * sizeof(seq_t));
    if (!primes || !primorials) return; /* allocation failed: caller may handle */

    bool *is_composite = (bool *)calloc(sz, sizeof(bool));
    if (!is_composite) return;

    prime_cnt = 0;
    seq_t current_primorial = 1;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt] = i;
            primorials[prime_cnt] = current_primorial; /* product of primes < i */
            current_primorial *= (seq_t)i;
            ++prime_cnt;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    cache_limit = limit;
}

/* binary search for prime index */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return lo; /* primes[lo] == p for valid prime */
}

/* arithmetic-derivative variant */
seq_t a(idx_t n)
{
    if (n <= 1u) return 0;

    /* build primes and primorials up to n */
    ensure_primes_upto((uint64_t)n);

    uint64_t n_orig = (uint64_t)n;
    uint64_t m = n_orig;
    seq_t   result = 0;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            seq_t a_p = primorials[i]; /* product of primes < p */
            seq_t term = ((seq_t)n_orig) / p; /* n/p */
            term *= (seq_t)e;
            term *= a_p;
            result += term;
        }
    }
    if (m > 1) { /* remaining prime factor */
        uint64_t p = m;
        size_t idx = prime_index(p);
        seq_t a_p = primorials[idx];
        seq_t term = ((seq_t)n_orig) / p;
        term *= a_p;
        result += term;
    }
    return result;
}

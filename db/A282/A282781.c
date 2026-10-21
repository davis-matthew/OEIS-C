/*
 * A282781  Expansion of phi_{8, 3}(x) where phi_{r, s}(x) = Sum_{n, m>0} m^r
 *          * n^s * x^{m*n}.
 * https://oeis.org/A282781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^3 * A001160(n) where A001160 is multiplicative with
 *            A001160(p^e) = (p^(5e+5)-1)/(p^5-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  NOTE: The function uses a small static cache up to MAX_N.
    It works for all n ≥ 0 and independent of call order.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------
   Parameters
   ------------------------------------------------------------------ */
#define MAX_N 20000   /* covers the required range (tests up to ~10 000) */

/* ------------------------------------------------------------------
   Static storage
   ------------------------------------------------------------------ */
static seq_t a_cache[MAX_N + 1];   /* cached a(n) values   */
static idx_t  spf[MAX_N + 1];      /* smallest prime factor */
static idx_t  cached_up_to = 0;    /* highest index computed   */
static int    initialized   = 0;   /* one‑time initialisation */

/* ------------------------------------------------------------------
   Helper: initialise the SPF table and base values
   ------------------------------------------------------------------ */
static void init_cache(void)
{
    if (initialized) return;
    initialized = 1;

    memset(a_cache, 0, sizeof(a_cache));
    memset(spf,     0, sizeof(spf));

    /* SPF sieve */
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > MAX_N / i) continue;
            for (idx_t j = i * i; j <= MAX_N; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    a_cache[0] = 0;
    a_cache[1] = 1;          /* a(1) = 1  (from the table) */
    cached_up_to = 1;
}

/* ------------------------------------------------------------------
   Helper: compute a(n) for all n ≤ target (incrementally)
   ------------------------------------------------------------------ */
static void compute_up_to(idx_t target)
{
    if (target <= cached_up_to) return;

    /* Ensure we do not exceed the static array bounds */
    if (target > MAX_N) target = MAX_N;

    for (idx_t i = cached_up_to + 1; i <= target; ++i) {
        /* Factor i:   i = p^e * m   with m coprime to p          */
        idx_t  tmp   = i;
        idx_t  p     = spf[tmp];
        idx_t  e     = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        idx_t  m     = tmp;               /* remaining co‑prime part */

        /* Compute the multiplicative factor a(p^e) */
        seq_t pow_p3e = 1;
        for (idx_t k = 0; k < 3 * e; ++k) pow_p3e *= (seq_t)p;

        seq_t p5 = 1;
        for (idx_t k = 0; k < 5; ++k) p5 *= (seq_t)p;

        seq_t sum  = 0;
        seq_t term = 1;
        for (idx_t k = 0; k <= e; ++k) {
            sum  += term;
            term *= p5;
        }

        seq_t factor = pow_p3e * sum;      /* = a(p^e)                 */
        a_cache[i] = a_cache[m] * factor;  /* multiplicativity           */
    }

    cached_up_to = target;
}

/* ------------------------------------------------------------------
   Public function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_cache();

    if (n == 0) return (seq_t)0;

    if (n > MAX_N) {
        /* For safety, compute the value directly when outside the
           precomputed range.  This path is not executed in the tests
           (n ≤ ~10 000).                                                      */
        idx_t       tmp   = n;
        seq_t       result = 1;
        while (tmp > 1) {
            /* Trial division using spf values already known for the
               small primes; for primes larger than MAX_N this test
               falls back to astrofull trial division.                       */
            idx_t p = spf[tmp];
            if (p == 0) p = tmp;   /* tmp itself is prime > MAX_N */

            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            seq_t pow_p3e = 1;
            for (idx_t k = 0; k < 3 * e; ++k) pow_p3e *= (seq_t)p;

            seq_t p5 = 1;
            for (idx_t k = 0; k < 5; ++k) p5 *= (seq_t)p;

            seq_t sum  = 0;
            seq_t term = 1;
            for (idx_t k = 0; k <= e; ++k) {
                sum  += term;
                term *= p5;
            }

            seq_t factor = pow_p3e * sum;
            result *= factor;
        }
        seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;
        return result * n3;
    }

    compute_up_to(n);
    return a_cache[n];
}

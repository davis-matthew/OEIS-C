/*
 * A374120  Numbers k such that A113177(k) and A276085(k) are both multiples
 *          of 3, where A113177 and A276085 are fully additive with a(p) =
 *          Fibonacci(p) and a(p) = p#/p, respectively.
 * https://oeis.org/A374120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of k such that A113177(k) and A276085(k) are both
 *            multiples of 3 (A113177 is additive with a(p)=Fib(p), A276085
 *            additive with a(p)=p#/p). k is included iff sum_{p^e||k}
 *            e*Fib(p) ≡ 0 (mod 3) and e2 + 2*e3 ≡ 0 (mod 3) where e2, e3 are
 *            exponents of primes 2 and 3. The implementation generat...
 * Range:     u8 n <= 24, u16 n <= 8891, u32 n <= 9967, u64 n <= 9967,
 *            u128 n <= 9967, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;             /* cached sequence values          */
static size_t seq_len = 0;            /* number of values cached         */
static size_t seq_cap = 0;            /* capacity of seq array           */
static idx_t  computed_limit = 0;     /* current sieve limit             */
static uint32_t *spf = NULL;          /* smallest prime factor array     */
static size_t  spf_cap = 0;           /* capacity of spf array           */
static uint32_t *primes = NULL;       /* list of primes up to limit     */
static size_t  primes_cnt = 0;        /* number of primes                */

/* fib modulo 3 pattern with period 8  */
static const int fib_mod3[8] = {0,1,1,2,0,2,2,1};

/* assemble primes and spf up to 'limit' */
static void build_sieve(idx_t limit)
{
    if (limit <= computed_limit && spf != NULL)
        return;                      /* already built up to this limit */

    /* free old arrays */
    free(spf);
    free(primes);

    spf_cap = (size_t)(limit + 1);
    spf = (uint32_t *)calloc(spf_cap, sizeof(uint32_t));
    if (!spf) return;

    size_t primes_size = 0, primes_alloc = 0;
    primes = NULL;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_size == primes_alloc) {
                primes_alloc = primes_alloc ? primes_alloc*2 : 64;
                primes = (uint32_t *)realloc(primes, primes_alloc * sizeof(uint32_t));
            }
            primes[primes_size++] = (uint32_t)i;
        }
        uint32_t pi;
        for (size_t j = 0; j < primes_size; ++j) {
            pi = primes[j];
            if ((idx_t)pi * i > limit) break;
            spf[pi * i] = pi;
            if (i % pi == 0) break;
        }
    }
    primes_cnt = primes_size;
    computed_limit = limit;
}

/* test if a number n satisfies the two congruences */
static bool qualifies(idx_t n)
{
    idx_t x = n;
    idx_t e2 = 0, e3 = 0;
    int sum1 = 0;                /* modulo 3 */
    while (x > 1) {
        uint32_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        if (p == 2) e2 += e;
        if (p == 3) e3 += e;

        int fmod3;
        if (p == 2)          fmod3 = 1;
        else if (p == 3)     fmod3 = 2;
        else
            fmod3 = fib_mod3[p & 7];    /* p % 8 */

        sum1 += (int)((e % 3) * (idx_t)fmod3);
        sum1 %= 3;
    }
    if (sum1 != 0) return false;
    if ((e2 + 2 * e3) % 3 != 0) return false;
    return true;
}

/* refill or extend the cached sequence up to at least 'need' terms */
static void ensure_sequence(idx_t need)
{
    if (seq_len >= (size_t)need)
        return;                           /* already sufficient */

    /* decide a new sieve limit */
    idx_t limit = (idx_t)(need * 10);
    if (limit < 64) limit = 64;

    while (true) {
        build_sieve(limit);

        /* allocate sequence buffer if needed */
        if (seq == NULL) {
            seq_cap = need > 64 ? need : 64;
            seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        }

        /* generate terms from scratch up to 'limit' */
        seq_len = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (qualifies(i)) {
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)i;
            }
        }

        if (seq_len >= (size_t)need)
            break;       /* enough terms found */

        /* not enough, double limit and try again */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, safety */
    ensure_sequence(n);
    return seq[n - 1];
}

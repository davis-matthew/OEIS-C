/*
 * A328847  Number of terms in Zeckendorf expansion needed to write the first
 *          Fibonacci based variant of arithmetic derivative of n.
 * https://oeis.org/A328847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms in Zeckendorf expansion of the first
 *            Fibonacci based variant of the arithmetic derivative of n
 * Range:     u8 n <= 9029, u16 n <= 9029, u32 n <= 9029, u64 n <= 9029,
 *            u128 n <= 9029, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses prime factorisation of n.  For each prime p^e in n the
   derivative contributes (n/p)*e copies of the Fibonacci number F_p.
   These copies are added in a base–Fibonacci representation and normalised
   to the Zeckendorf form.  The number of 1–bits in that representation
   equals a(n).  All arithmetic is performed with integer types supplied
   by the build; no intermediate values overflow the chosen seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* maximum n that is ever queried */
#define MAX_N 10000
/* upper bound for prime indices that can appear in the computation;
   we allocate some extra space for carries that arise during normalization. */
#define MAX_IDX 20000

/* sieve primes up to MAX_N */
static uint32_t primes[MAX_N];
static unsigned int prime_cnt = 0;
static void build_primes(void)
{
    static int ready = 0;
    if (ready) return;
    ready = 1;
    int ispt[MAX_N + 1];
    memset(ispt, 1, sizeof(ispt));
    ispt[0] = ispt[1] = 0;
    for (int i = 2; i * i <= MAX_N; ++i)
        if (ispt[i])
            for (int j = i * i; j <= MAX_N; j += i)
                ispt[j] = 0;
    for (int i = 2; i <= MAX_N; ++i)
        if (ispt[i]) primes[prime_cnt++] = i;
}

/* True if n > 1 and has no divisor other than 1 and itself.
   Used only for the trivial check of n<=1. */
static inline int is_prime_check(uint64_t n)
{
    if (n < 2) return 0;
    for (unsigned int i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i)
        if (n % primes[i] == 0) return 0;
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(0) = a(1) = 0 */

    build_primes();

    /* factorise n */
    uint64_t orig = (uint64_t)n;
    uint64_t temp = orig;

    /* small fixed-size storage for factorisation */
    uint64_t fac[16];
    int exp[16];
    int nf = 0;

    for (unsigned int i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            int e = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++e;
            }
            fac[nf]   = primes[i];
            exp[nf++] = e;
        }
    }
    if (temp > 1) {   /* remaining prime factor */
        fac[nf]   = temp;
        exp[nf++] = 1;
    }

    /* array that holds the raw counts of Fibonacci terms.
       indices correspond to Fibonacci index (F_0 = 0, F_1 = 1, …).
       We ignore indices 0 and 1 because they never appear in Zeckendorf. */
    static int cnt[MAX_IDX + 1];
    memset(cnt, 0, sizeof(cnt));

    /* accumulate copies of each F_p   by m = (n/p)*e */
    for (int i = 0; i < nf; ++i) {
        uint64_t p  = fac[i];
        int e       = exp[i];
        uint64_t m  = (orig / p) * (uint64_t)e;   /* m <= 53248 for n <= 10000 */
        if (p <= MAX_IDX)
            cnt[(int)p] += (int)m;
    }

    /* normalise to Zeckendorf: all counts 0 or 1, and no consecutive 1s.
       The following loop repeats until no changes are made. */
    int size = MAX_IDX + 1;
    int changed;
    do {
        changed = 0;
        for (int i = 2; i < size - 1; ++i) {
            /* handle counts >= 2 */
            if (cnt[i] >= 2) {
                int pairs = cnt[i] >> 1;   /* cnt[i]/2   */
                cnt[i]   -= (pairs << 1);  /* subtract 2*pairs */
                if (i == 2) {
                    cnt[3] += pairs;
                } else if (i == 3) {
                    cnt[4] += pairs;
                    cnt[2] += pairs;
                } else {
                    cnt[i + 1] += pairs;
                    cnt[i - 2] += pairs;
                }
                changed = 1;
            }
            /* handle consecutive 1s */
            if (cnt[i] == 1 && i > 2 && cnt[i - 1] == 1) {
                cnt[i]      = 0;
                cnt[i - 1]  = 0;
                cnt[i + 1] += 1;
                changed = 1;
            }
        }
    } while (changed);

    /* count the number of 1's */
    seq_t result = 0;
    for (int i = 2; i < size; ++i)
        result += cnt[i];

    return result;
}

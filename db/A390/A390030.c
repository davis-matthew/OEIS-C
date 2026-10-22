/*
 * A390030  Number of perfect overpartitions of n.
 * https://oeis.org/A390030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=0..Ω(n+1)} Sum_{v=r..Ω(n+1)} binom(v,r)*f_v(n+1)
 *            where f_v(N) is the number of ordered factorizations of N having
 *            v copies of 2 and Ω(N) is the total number of prime factors
 *            counted with multiplicity
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 10001          /* maximum N = n+1 we support (n <= 10000)   */
#define MAX_V 13             /* Ω(N) <= 13 for N <= 10001                */

/* forward declaration of small_binom (computed for v <= MAX_V)                 */
static seq_t small_binom(idx_t n, idx_t k);

/* Helper arrays – static to keep them for all calls                            */
static uint16_t spf[MAX_N + 1];      /* smallest prime factor for each N   */
static uint8_t omega_arr[MAX_N + 1]; /* Ω(N) for each N                    */
static seq_t dp[MAX_N + 1][MAX_V + 1];   /* f_v(N) counts                    */
static bool initialized = false;

/* Compute small binomial coefficient: C(n,k) (n,k <= MAX_V).                  */
static seq_t small_binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* One‑time initializer: sieve, Ω array and f_v counts.                         */
static void init_once(void)
{
    /* Smallest prime factor sieve.                                           */
    for (idx_t i = 0; i <= MAX_N; ++i) spf[i] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint16_t)i;
            if (i <= MAX_N / i) {
                for (idx_t j = i * i; j <= MAX_N; j += i)
                    if (spf[j] == 0) spf[j] = (uint16_t)i;
            }
        }
    }

    /* Ω(N) – total number of prime factors with multiplicity.               */
    omega_arr[1] = 0;
    for (idx_t x = 2; x <= MAX_N; ++x) {
        idx_t y = x;
        unsigned c = 0;
        while (y > 1) {
            idx_t p = spf[y];
            while (y % p == 0) {
                y /= p;
                ++c;
            }
        }
        omega_arr[x] = (uint8_t)c;
    }

    /* Zero all dp entries, set dp[1][0] = 1, others 0.                       */
    for (idx_t i = 0; i <= MAX_N; ++i)
        for (unsigned v = 0; v <= MAX_V; ++v)
            dp[i][v] = (seq_t)0;
    dp[1][0] = (seq_t)1;

    /* Compute f_v(N) for N = 2 .. MAX_N by dynamic programming.             */
    for (idx_t N = 2; N <= MAX_N; ++N) {
        /* Enumerate divisors d >= 2.                                         */
        for (idx_t d = 2; d * d <= N; ++d) {
            if (N % d == 0) {
                idx_t rest = N / d;
                bool isTwo = (d == 2);
                if (isTwo) {
                    for (unsigned v = 1; v <= MAX_V; ++v)
                        dp[N][v] += dp[rest][v - 1];
                } else {
                    for (unsigned v = 0; v <= MAX_V; ++v)
                        dp[N][v] += dp[rest][v];
                }
                if (d * d != N) {
                    idx_t d2 = N / d;     /* second divisor */
                    rest = N / d2;          /* which is d */
                    isTwo = (d2 == 2);
                    if (isTwo) {
                        for (unsigned v = 1; v <= MAX_V; ++v)
                            dp[N][v] += dp[rest][v - 1];
                    } else {
                        for (unsigned v = 0; v <= MAX_V; ++v)
                            dp[N][v] += dp[rest][v];
                    }
                }
            }
        }
        /* Handle the divisor equal to N itself.                               */
        bool isTwo = (N == 2);
        idx_t rest = 1;
        if (isTwo) {
            for (unsigned v = 1; v <= MAX_V; ++v)
                dp[N][v] += dp[rest][v - 1];
        } else {
            for (unsigned v = 0; v <= MAX_V; ++v)
                dp[N][v] += dp[rest][v];
        }
    }

    initialized = true;
}

/* Main sequence function                                                             */
seq_t a(idx_t n)
{
    if (!initialized) init_once();

    idx_t N = n + (idx_t)1;          /* N = n+1                                                */
    if (N > MAX_N)                    /* not expected but guard                                 */
        return (seq_t)0;

    idx_t s = omega_arr[N];           /* Ω(N)                                                     */
    seq_t total = (seq_t)0;

    for (idx_t r = 0; r <= s; ++r) {
        for (idx_t v = r; v <= s; ++v) {
            seq_t bin = small_binom(v, r);
            total += bin * dp[N][v];
        }
    }
    return total;
}
